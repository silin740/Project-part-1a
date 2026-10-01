#include "GameEngine.h"
#include "CollisionObject.h"
#include "DrawContext.h"
#include <SFML/Graphics.hpp>
#include "GameContext.h"
#include "GraphicsObject.h"
#include <algorithm>


/// @brief
namespace CMPUT350 {
#include "FontData.h"

GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& name) {
    // Sample font loading code

    mWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode({width, height}), name);
    mWindow->setFramerateLimit(30);

    mFont = std::make_shared<sf::Font>();

    if (!mFont->openFromMemory(&_font, _font_len))
    {
        fprintf(stderr, "WARNING: Font did not load.\n");
    }

    mDrawContext = std::make_shared<DrawContext>(mWindow, mFont);
}

GameEngine::~GameEngine() {
    if (mWindow && mWindow->isOpen()) {
        mWindow->close();
    }
}

void GameEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    mNewGameObjects.push_back(std::move(gameObject));
}

bool GameEngine::ProcessEvents(GameContext* context) {
    while (const std::optional event = mWindow->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            mWindow->close();
            return false;
        }

        if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>()) {
            for (const auto& gameObject : mGameObjects) {
                if (gameObject->HandleKeyEvent(
                        context, static_cast<char>(keyPressed->unicode))) {
                    break;
                }
            }
        }
    }

    return mWindow->isOpen();
}

/**
 * @method Run
 * @arguments None
 * @description Gives control to the game engine. Will not return until the game window is closed or
 * all objects have been destroyed.
 */
void GameEngine::Run()
{
    GameContext context;
    context.mEngineView = this;
    context.ScreenContext = mDrawContext.get();

    while (mWindow->isOpen())
    {
        // 0. Remove dead objects
        std::erase_if(mGameObjects,
                      [](const std::shared_ptr<GameObject>& gameObject)
                      {
                          return !gameObject->IsAlive();
                      });

        // 1. Activate objects added during the previous frame
        for (const auto& gameObject : mNewGameObjects)
        {
            gameObject->Initialize(&context);
            mGameObjects.push_back(gameObject);
        }

        mNewGameObjects.clear();

        // 2. Process events
        if (!ProcessEvents(&context))
        {
            break;
        }

        // 3. Update game objects
        for (const auto& gameObject : mGameObjects)
        {
            gameObject->Update(&context);
        }

        // 4. Process collision events
        for (size_t i = 0; i < mGameObjects.size(); ++i)
        {
            auto first =
                std::dynamic_pointer_cast<CollisionObject>(mGameObjects[i]);

            if (!first)
            {
                continue;
            }

            for (size_t j = i + 1; j < mGameObjects.size(); ++j)
            {
                auto second =
                    std::dynamic_pointer_cast<CollisionObject>(mGameObjects[j]);

                if (!second)
                {
                    continue;
                }

                const Rect& firstBounds = first->GetBounds();
                const Rect& secondBounds = second->GetBounds();

                const bool overlaps =
                    firstBounds.topLeft.x < secondBounds.topLeft.x + secondBounds.width &&
                    firstBounds.topLeft.x + firstBounds.width > secondBounds.topLeft.x &&
                    firstBounds.topLeft.y < secondBounds.topLeft.y + secondBounds.height &&
                    firstBounds.topLeft.y + firstBounds.height > secondBounds.topLeft.y;

                if (overlaps)
                {
                    first->CollisionEnter(second);
                    second->CollisionEnter(first);
                }
            }
        }

        // 5. Late updates
        for (const auto& gameObject : mGameObjects)
        {
            gameObject->LateUpdate(&context);
        }

        // Clear window
        mWindow->clear();

        // 6. Render background
        for (const auto& gameObject : mGameObjects)
        {
            auto graphicsObject =
                std::dynamic_pointer_cast<GraphicsObject>(gameObject);

            if (graphicsObject)
            {
                graphicsObject->RenderBackground(&context);
            }
        }

        // 7. Render foreground
        for (const auto& gameObject : mGameObjects)
        {
            auto graphicsObject =
                std::dynamic_pointer_cast<GraphicsObject>(gameObject);

            if (graphicsObject)
            {
                graphicsObject->RenderForeground(&context);
            }
        }

        // Render UI
        for (const auto& gameObject : mGameObjects)
        {
            gameObject->RenderUI(&context);
        }

        // Actually render to window
        mWindow->display();
    }
}

// Sample code for processing events

// bool GameEngine::ProcessEvents(GameContext *context)
//{
//	while (const std::optional event = mWindow->pollEvent())
//	{
//		if (event->is<sf::Event::Closed>())
//		{
//		}
//		else if (event->is<sf::Event::Resized>())
//		{
//		}
//		else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
//		{
//			// use keyPressed->unicode to get character
//		}
//	}
// }

}  // namespace CMPUT350
