#include "AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../DebugLayer/Debug.h"
#include "../../Imgui/imgui.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

AABBCollider::AABBCollider(GameObject& gameObject) :
    Collider(gameObject),
    mAABB(),
    mDefaultMin(Vector3::zero),
    mDefaultMax(Vector3::zero),
    mIsRenderCollision(true),
    mLoadedProperties(false) {
}

AABBCollider::~AABBCollider() = default;

void AABBCollider::start() {
    Collider::start();

    //ファイルから値を読み込んでいないなら頂点から形成する
    if (!mLoadedProperties) {
        auto meshComponent = getComponent<MeshComponent>();
        if (meshComponent) {
            const auto& mesh = meshComponent->getMesh();
            //メッシュ情報からAABBを作成する
            createAABB(mesh);
        }
    }

    //早速transformが変わっているかもしれないから更新する
    updateAABB();
    //最新のAABBの点を計算する
    updatePoints();
}

void AABBCollider::lateUpdate() {
    //当たり判定が自動化設定されているなら
    if (mIsAutoUpdate) {
        updateAABB();
    }

    //AABBの点を更新する
    updatePoints();

    //当たり判定を可視化する
    if (mIsRenderCollision) {
        renderCollision();
    }
}

void AABBCollider::onEnable(bool value) {
    setRenderCollision(value);
}

void AABBCollider::loadProperties(const rapidjson::Value& inObj) {
    if (JsonHelper::getVector3(inObj, "min", &mDefaultMin)) {
        mAABB.min = mDefaultMin;
        mLoadedProperties = true;
    }
    if (JsonHelper::getVector3(inObj, "max", &mDefaultMax)) {
        mAABB.max = mDefaultMax;
        mLoadedProperties = true;
    }
    JsonHelper::getBool(inObj, "isRenderCollision", &mIsRenderCollision);
}

void AABBCollider::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setVector3(alloc, inObj, "min", mDefaultMin);
    JsonHelper::setVector3(alloc, inObj, "max", mDefaultMax);
    JsonHelper::setBool(alloc, inObj, "isRenderCollision", mIsRenderCollision);
}

void AABBCollider::drawInspector() {
    Collider::drawInspector();

    auto& min = mAABB.min;
    float mi[3] = { min.x, min.y, min.z };
    ImGui::SliderFloat3("Min", mi, -10000.f, 10000.f);
    auto& max = mAABB.max;
    float ma[3] = { max.x, max.y, max.z };
    ImGui::SliderFloat3("Max", ma, -10000.f, 10000.f);
    ImGui::Checkbox("IsRenderCollision", &mIsRenderCollision);
}

void AABBCollider::set(const Vector3& min, const Vector3& max) {
    mAABB.min = min;
    mAABB.max = max;

    //デフォルト点を修正する
    computeDefaultPoint();

    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const AABB& AABBCollider::getAABB() const {
    return mAABB;
}

const std::array<Vector3, 8>& AABBCollider::getBoxPoints() const {
    return mPoints;
}

std::array<std::pair<Vector3, Vector3>, 6> AABBCollider::getBoxSurfacesCenterAndNormal() const {
    return std::array<std::pair<Vector3, Vector3>, 6> {
        //面の中心位置と法線を格納する
        std::make_pair((mPoints[3] + mPoints[0]) / 2.f, Vector3::down),
        std::make_pair((mPoints[7] + mPoints[4]) / 2.f, Vector3::up),
        std::make_pair((mPoints[3] + mPoints[5]) / 2.f, Vector3::right),
        std::make_pair((mPoints[2] + mPoints[4]) / 2.f, Vector3::left),
        std::make_pair((mPoints[5] + mPoints[0]) / 2.f, Vector3::back),
        std::make_pair((mPoints[7] + mPoints[2]) / 2.f, Vector3::forward)
    };
}

void AABBCollider::setRenderCollision(bool value) {
    mIsRenderCollision = value;
}

void AABBCollider::createAABB(const IMesh& mesh) {
    //すべてのメッシュからAABBを作成する
    for (size_t i = 0; i < mesh.getMeshCount(); i++) {
        Vector3 min, max;
        computeMinMax(min, max, mesh.getMeshVertices(i));

        //当たり判定更新
        mAABB.updateMinMax(min);
        mAABB.updateMinMax(max);
    }
    mDefaultMin = mAABB.min;
    mDefaultMax = mAABB.max;
}

void AABBCollider::computeMinMax(Vector3& outMin, Vector3& outMax, const MeshVertices& meshVertices) {
    auto min = Vector3::one * Math::infinity;
    auto max = Vector3::one * Math::negInfinity;

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

void AABBCollider::updateAABB() {
    const auto& t = transform();
    const auto& pos = t.getPosition();
    const auto& scale = t.getScale();
    //回転を適用するために一時的なAABBを作成する
    AABB aabb(mDefaultMin - t.getPivot(), mDefaultMax - t.getPivot());
    //回転を適用する
    aabb.rotate(t.getRotation());
    //既存のAABBを更新する
    mAABB.min = aabb.min * scale + pos;
    mAABB.max = aabb.max * scale + pos;
}

void AABBCollider::updatePoints() {
    const auto& min = mAABB.min;
    const auto& max = mAABB.max;
    mPoints[0] = min;
    mPoints[1] = Vector3(max.x, min.y, min.z);
    mPoints[2] = Vector3(min.x, min.y, max.z);
    mPoints[3] = Vector3(max.x, min.y, max.z);
    mPoints[4] = Vector3(min.x, max.y, min.z);
    mPoints[5] = Vector3(max.x, max.y, min.z);
    mPoints[6] = Vector3(min.x, max.y, max.z);
    mPoints[7] = max;
}

void AABBCollider::computeDefaultPoint() {
    const auto& t = transform();
    const auto& pos = t.getPosition();
    const auto& scale = t.getScale();

    //updateAABBの逆順に計算していく
    auto min = Vector3(
        mAABB.min.x / scale.x - pos.x,
        mAABB.min.y / scale.y - pos.y,
        mAABB.min.z / scale.z - pos.z
    );
    auto max = Vector3(
        mAABB.max.x / scale.x - pos.x,
        mAABB.max.y / scale.y - pos.y,
        mAABB.max.z / scale.z - pos.z
    );
    AABB aabb(min + t.getPivot(), max + t.getPivot());

    //更新する
    mDefaultMin = aabb.min;
    mDefaultMax = aabb.max;
}

void AABBCollider::renderCollision() {
#ifdef _DEBUG
    //デバッグ時のみ当たり判定を表示
    Debug::renderLine(mPoints[0], mPoints[1], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[0], mPoints[2], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[2], mPoints[3], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[1], mPoints[3], ColorPalette::lightGreen);

    Debug::renderLine(mPoints[4], mPoints[5], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[4], mPoints[6], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[6], mPoints[7], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[5], mPoints[7], ColorPalette::lightGreen);

    Debug::renderLine(mPoints[0], mPoints[4], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[1], mPoints[5], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[2], mPoints[6], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[3], mPoints[7], ColorPalette::lightGreen);
#endif // _DEBUG
}
