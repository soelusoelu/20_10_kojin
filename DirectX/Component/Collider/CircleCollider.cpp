#include "CircleCollider.h"
#include "../Sprite/SpriteComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../Transform/Transform2D.h"

CircleCollider::CircleCollider(GameObject& gameObject) :
    Collider(gameObject),
    mSprite(nullptr),
    mCircle(std::make_unique<Circle>()) {
}

CircleCollider::~CircleCollider() = default;

void CircleCollider::start() {
    Collider::start();

    mSprite = getComponent<SpriteComponent>();
    circleUpdate();
}

void CircleCollider::update() {
    Collider::update();

    if (!mIsAutoUpdate) {
        return;
    }

    circleUpdate();
}

void CircleCollider::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    Collider::drawDebugInfo(inspect);

    inspect->emplace_back("Center", mCircle->center);
    inspect->emplace_back("Radius", mCircle->radius);
}

void CircleCollider::set(const Vector2& center, float radius) {
    mCircle->set(center, radius);
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const Circle& CircleCollider::getCircle() const {
    return *mCircle;
}

void CircleCollider::circleUpdate() {
    if (!mSprite) {
        return;
    }

    const auto& t = mSprite->transform();
    const auto& size = mSprite->getTextureSize();
    auto maxSize = Math::Max<float>(size.x, size.y);
    const auto& scale = t.getScale();
    auto maxScale = Math::Max<float>(scale.x, scale.y);
    auto radius = maxSize / 2.f * maxScale;

    mCircle->set(t.getPosition(), radius);
}
