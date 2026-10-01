#include "Bullet.h"

#include "DrawContext.h"
#include "Enemy.h"
#include "Player.h"

Bullet::Bullet(CMPUT350::Point2D location, CMPUT350::Point2D heading, bool player)
    : mLocation(location), mPreviousLocation(location), mHeading(heading), mPlayerBullet(player) {}

bool Bullet::IsPlayerBullet() { return mPlayerBullet; }
void Bullet::Initialize(CMPUT350::GameContext*) {}

void Bullet::Update(CMPUT350::GameContext* context) {
    mPreviousLocation = mLocation;
    mLocation += mHeading;
    const float margin = 20.f;
    if (mLocation.x < -margin || mLocation.x > context->ScreenContext->GetWindowWidth() + margin ||
        mLocation.y < -margin || mLocation.y > context->ScreenContext->GetWindowHeight() + margin) {
        Kill();
    }
}

void Bullet::LateUpdate(CMPUT350::GameContext*) {}
bool Bullet::HandleKeyEvent(CMPUT350::GameContext*, char) { return false; }
void Bullet::RenderBackground(CMPUT350::GameContext*) {}

void Bullet::RenderForeground(CMPUT350::GameContext* context) {
    const CMPUT350::RGBColor colour = mPlayerBullet ? CMPUT350::Colors::green : CMPUT350::Colors::yellow;
    context->ScreenContext->DrawLine(mPreviousLocation, mLocation, 4.f, colour);
}

void Bullet::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj) {
    if (std::dynamic_pointer_cast<Bullet>(obj)) return;
    if (mPlayerBullet && std::dynamic_pointer_cast<Player>(obj)) return;
    if (!mPlayerBullet && std::dynamic_pointer_cast<Enemy>(obj)) return;
    Kill();
}

void Bullet::Kill() { mAlive = false; }
bool Bullet::IsAlive() const { return mAlive; }

const CMPUT350::Rect& Bullet::GetBounds() {
    mBounds = CMPUT350::Rect(mPreviousLocation, mLocation);
    mBounds |= CMPUT350::Rect(mPreviousLocation, 2.f);
    mBounds |= CMPUT350::Rect(mLocation, 2.f);
    return mBounds;
}
