#include "SphereCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../Transform/Transform3D.h"

SphereCollider::SphereCollider(GameObject& gameObject) :
    Collider(gameObject),
    mSphere(std::make_unique<Sphere>()),
    mDefaultCenter(Vector3::zero),
    mDefaultRadius(0.f) {
}

SphereCollider::~SphereCollider() = default;

void SphereCollider::start() {
    Collider::start();

    auto mesh = getComponent<MeshComponent>();
    if (mesh) {
        mSphere->center = mesh->getCenter();
        mSphere->radius = mesh->getRadius();
        mDefaultCenter = mSphere->center;
        mDefaultRadius = mSphere->radius;
    }
}

void SphereCollider::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    Collider::drawDebugInfo(inspect);

    inspect->emplace_back("Center", mSphere->center);
    inspect->emplace_back("Radius", mSphere->radius);
}

void SphereCollider::onUpdateWorldTransform() {
    Collider::onUpdateWorldTransform();

    if (!mIsAutoUpdate) {
        return;
    }

    auto center = mDefaultCenter + transform().getPosition();
    auto scale = transform().getScale();
    auto maxScaleValue = Math::Max<float>(scale.x, Math::Max<float>(scale.y, scale.z));
    auto radius = mDefaultRadius * maxScaleValue;

    mSphere->set(center, radius);
}

void SphereCollider::set(const Vector3& center, float radius) {
    mSphere->set(center, radius);
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const Sphere& SphereCollider::getSphere() const {
    return *mSphere;
}
