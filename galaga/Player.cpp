#include <algorithm>
#include <cctype>

#include "Bullet.h"
#include "DrawContext.h"
#include "GameContext.h"
#include "Player.h"

Player::Player(CMPUT350::Point2D loc) : mLocation(loc) {}

void Player::Initialize(CMPUT350::GameContext*) {}

void Player::Update(CMPUT350::GameContext* context) {
    const float halfWidth = 20.f;
    mLocation.x = std::clamp(mLocation.x, halfWidth,
                             static_cast<float>(context->ScreenContext->GetWindowWidth()) - halfWidth);
}

void Player::LateUpdate(CMPUT350::GameContext*) {}

bool Player::HandleKeyEvent(CMPUT350::GameContext* context, char key) {
    const char input = static_cast<char>(std::tolower(static_cast<unsigned char>(key)));
    if (input == 'a') {
        mLocation.x -= 15.f;
        return true;
    }
    if (input == 'd') {
        mLocation.x += 15.f;
        return true;
    }
    if (input != ' ') return false;

    for (auto& bulletSlot : mBullets) {
        if (!bulletSlot.expired()) continue;
        auto bullet = std::make_shared<Bullet>(
            CMPUT350::Point2D(mLocation.x, mLocation.y - 22.f),
            CMPUT350::Point2D(0.f, -18.f), true);
        bulletSlot = bullet;
        context->mEngineView->AddGameObject(bullet);
        return true;
    }
    return true;
}

void Player::RenderBackground(CMPUT350::GameContext*) {}

void Player::RenderForeground(CMPUT350::GameContext* context) {
    auto* screen = context->ScreenContext;
    screen->DrawRect(CMPUT350::Rect(mLocation.x - 20.f, mLocation.y - 20.f, 40.f, 40.f),
                     CMPUT350::Colors::cyan);
    screen->DrawLine({mLocation.x - 12.f, mLocation.y + 8.f},
                     {mLocation.x, mLocation.y - 18.f}, 4.f, CMPUT350::Colors::white);
    screen->DrawLine({mLocation.x, mLocation.y - 18.f},
                     {mLocation.x + 12.f, mLocation.y + 8.f}, 4.f, CMPUT350::Colors::white);
}

void Player::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>&) {}

void Player::Kill() { mAlive = false; }

bool Player::IsAlive() const { return mAlive; }

const CMPUT350::Rect& Player::GetBounds() {
    mBounds = CMPUT350::Rect(mLocation.x - 20.f, mLocation.y - 20.f, 40.f, 40.f);
    return mBounds;
}
