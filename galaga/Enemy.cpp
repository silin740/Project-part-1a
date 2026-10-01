#include "Bullet.h"
#include "DrawContext.h"
#include "Enemy.h"

Enemy::Enemy(CMPUT350::Point2D loc) : mLocation(loc) {}

void Enemy::Initialize(CMPUT350::GameContext*) {}
void Enemy::Update(CMPUT350::GameContext*) {}
void Enemy::LateUpdate(CMPUT350::GameContext*) {}
bool Enemy::HandleKeyEvent(CMPUT350::GameContext*, char) { return false; }
void Enemy::RenderBackground(CMPUT350::GameContext*) {}

void Enemy::RenderForeground(CMPUT350::GameContext* context) {
    auto* screen = context->ScreenContext;
    screen->DrawRect(CMPUT350::Rect(mLocation.x - 20.f, mLocation.y - 20.f, 40.f, 40.f),
                     CMPUT350::Colors::magenta);
    screen->FrameRect(CMPUT350::Rect(mLocation.x - 20.f, mLocation.y - 20.f, 40.f, 40.f),
                      2.f, CMPUT350::Colors::yellow);
}

void Enemy::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj) {
    const auto bullet = std::dynamic_pointer_cast<Bullet>(obj);
    if (bullet && bullet->IsPlayerBullet()) Kill();
}

void Enemy::Kill() { mAlive = false; }
bool Enemy::IsAlive() const { return mAlive; }

const CMPUT350::Rect& Enemy::GetBounds() {
    mBounds = CMPUT350::Rect(mLocation.x - 20.f, mLocation.y - 20.f, 40.f, 40.f);
    return mBounds;
}
