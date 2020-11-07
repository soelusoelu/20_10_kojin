#include "SphereCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../Imgui/imgui.h"
#include "../../Transform/Transform3D.h"

SphereCollider::SphereCollider(GameObject& gameObject) :
    Collider(gameObject),
    mSphere(),
    mDefaultCenter(Vector3::zero),
    mDefaultRadius(0.f) {
}

SphereCollider::~SphereCollider() = default;

void SphereCollider::start() {
    Collider::start();

    auto meshComponent = getComponent<MeshComponent>();
    if (meshComponent) {
        const auto& mesh = meshComponent->getMesh();
        //メッシュ情報から球を作成する
        createSphere(mesh);
    }
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

    mSphere.center = center;
    mSphere.radius = radius;
}

void SphereCollider::drawInspector() {
    Collider::drawInspector();

    auto& center = mSphere.center;
    float c[3] = { center.x, center.y, center.z };
    ImGui::SliderFloat3("Center", c, FLT_MIN, FLT_MAX);
    ImGui::SliderFloat("Radius", &mSphere.radius, FLT_MIN, FLT_MAX);
}

void SphereCollider::set(const Vector3& center, float radius) {
    mSphere.center = center;
    mSphere.radius = radius;
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const Sphere& SphereCollider::getSphere() const {
    return mSphere;
}

void SphereCollider::createSphere(const IMesh& mesh) {
    //すべてのメッシュから球を作成する
    auto min = Vector3::one * Math::infinity;
    auto max = Vector3::one * Math::negInfinity;
    for (size_t i = 0; i < mesh.getMeshCount(); i++) {
        computeCenterRadius(min, max, mesh.getMeshVertices(i));
    }
    mDefaultCenter = (min + max) / 2.f;
    mDefaultRadius = (max - min).length() / 2.f;
    mSphere.center = mDefaultCenter;
    mSphere.radius = mDefaultRadius;
}

void SphereCollider::computeCenterRadius(Vector3& outMin, Vector3& outMax, const MeshVertices& meshVertices) {
    //中心位置計算のための最小、最大位置
    auto min = outMin;
    auto max = outMax;

    //メッシュ情報から最小、最大点を割り出す
    for (size_t i = 0; i < meshVertices.size(); ++i) {
        const auto& vertices = meshVertices[i];
        const auto& p = vertices.pos;
        if (p.x < min.x) {
            min.x = p.x;
        }
        if (p.x > max.x) {
            max.x = p.x;
        }
        if (p.y < min.y) {
            min.y = p.y;
        }
        if (p.y > max.y) {
            max.y = p.y;
        }
        if (p.z < min.z) {
            min.z = p.z;
        }
        if (p.z > max.z) {
            max.z = p.z;
        }
    }

    outMin = min;
    outMax = max;
}
