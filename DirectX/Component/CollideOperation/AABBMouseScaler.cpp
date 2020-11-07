#include "AABBMouseScaler.h"
#include "../Camera/Camera.h"
#include "../Collider/AABBCollider.h"
#include "../../Collision/Collision.h"
#include "../../DebugLayer/Debug.h"
#include "../../Device/Time.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Imgui/imgui.h"
#include "../../Input/Input.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

AABBMouseScaler::AABBMouseScaler(GameObject& gameObject)
    : Component(gameObject)
    , mCamera(nullptr)
    , mCollider(nullptr)
    , mEditPointRadius(0.f)
    , mCollisionExpantionAmount(0.f)
    , mSelectedEditPoint(false)
{
}

AABBMouseScaler::~AABBMouseScaler() = default;

void AABBMouseScaler::start() {
    const auto& gameObjectManager = gameObject().getGameObjectManager();
    mCamera = gameObjectManager.find("Camera")->componentManager().getComponent<Camera>();
}

void AABBMouseScaler::update() {
    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();

    //マウスの左ボタンを押した瞬間だったら
    if (Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        //編集点を選択する
        selectBoxPoint();
    }
    //マウスの左ボタンを押し続けていたら
    if (mouse.getMouseButton(MouseCode::LeftButton)) {
        //編集ポイントを選択していたら
        if (mSelectedEditPoint) {
            calculateNewBoxPoint();
        }
    }
    //マウスの左ボタンを離した瞬間だったら
    if (mouse.getMouseButtonUp(MouseCode::LeftButton)) {
        mSelectedEditPoint = false;
    }

    if (mCollider) {
        const auto& surfaces = mCollider->getBoxSurfacesCenterAndNormal();
        for (const auto& surface : surfaces) {
            Debug::renderPoint(surface.first);
        }
    }
}

void AABBMouseScaler::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getFloat(inObj, "editPointRadius", &mEditPointRadius);
    JsonHelper::getFloat(inObj, "collisionExpantionAmount", &mCollisionExpantionAmount);
}

void AABBMouseScaler::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setFloat(alloc, inObj, "editPointRadius", mEditPointRadius);
    JsonHelper::setFloat(alloc, inObj, "collisionExpantionAmount", mCollisionExpantionAmount);
}

void AABBMouseScaler::drawInspector() {
    ImGui::SliderFloat("EditPointRadius", &mEditPointRadius, 0.1f, 3.f);
    ImGui::SliderFloat("CollisionExpantionAmount", &mCollisionExpantionAmount, 0.1f, 10.f);
}

void AABBMouseScaler::setAABB(const std::shared_ptr<AABBCollider>& aabb) {
    mCollider = aabb;
}

bool AABBMouseScaler::editing() const {
    return mSelectedEditPoint;
}

void AABBMouseScaler::selectBoxPoint() {
    if (!mCollider) {
        return;
    }

    //AABBすべての面と法線を取得する
    const auto& surfaces = mCollider->getBoxSurfacesCenterAndNormal();

    //すべての面の中心位置を球に見立ててレイと球の当たり判定を行う
    Ray cameraToMousePos = mCamera->screenToRay(Input::mouse().getMousePosition());
    Vector3 intersectPoint;
    for (const auto& surface : surfaces) {
        if (Intersect::intersectRaySphere(cameraToMousePos, { surface.first, mEditPointRadius }, intersectPoint)) {
            mSelectSurface = surface;
            mSelectedEditPoint = true;
            return;
        }
    }
}

void AABBMouseScaler::calculateNewBoxPoint() {
    //取得した面から法線を取得
    const auto& surfaceNormal = mSelectSurface.second;

    //マウスの移動量を3次元に換算する
    const auto& mouseMoveAmount = Input::mouse().getMouseMoveAmount();
    auto dir = Vector3(
        mouseMoveAmount.x,
        -mouseMoveAmount.y,
        (mouseMoveAmount.x + -mouseMoveAmount.y) / 2.f
    );

    //カメラの向きを考慮する
    dir = Vector3::transform(dir, mCamera->transform().getRotation());

    //面の方向と移動量を決める
    float amount = Vector3::dot(dir, surfaceNormal);

    //初期位置にマウスの移動量で更新する
    mSelectSurface.first += surfaceNormal * amount * Time::deltaTime * mCollisionExpantionAmount;

    //当たり判定更新
    updateBox(mSelectSurface.first, surfaceNormal);
}

void AABBMouseScaler::updateBox(const Vector3& calcPoint, const Vector3& surfaceNormal) {
    //計算用の一時的AABBを作成する
    AABB temp(mCollider->getAABB());

    const auto& aabb = mCollider->getAABB();
    if (Vector3::equal(surfaceNormal, Vector3::right)) {
        if (calcPoint.x > aabb.max.x) {
            temp.updateMinMax(calcPoint);
        } else {
            temp.max.x = calcPoint.x;
        }
    } else if (Vector3::equal(surfaceNormal, Vector3::left)) {
        if (calcPoint.x < aabb.min.x) {
            temp.updateMinMax(calcPoint);
        } else {
            temp.min.x = calcPoint.x;
        }
    } else if (Vector3::equal(surfaceNormal, Vector3::up)) {
        if (calcPoint.y > aabb.max.y) {
            temp.updateMinMax(calcPoint);
        } else {
            temp.max.y = calcPoint.y;
        }
    } else if (Vector3::equal(surfaceNormal, Vector3::down)) {
        if (calcPoint.y < aabb.min.y) {
            temp.updateMinMax(calcPoint);
        } else {
            temp.min.y = calcPoint.y;
        }
    } else if (Vector3::equal(surfaceNormal, Vector3::forward)) {
        if (calcPoint.z > aabb.max.z) {
            temp.updateMinMax(calcPoint);
        } else {
            temp.max.z = calcPoint.z;
        }
    } else if (Vector3::equal(surfaceNormal, Vector3::back)) {
        if (calcPoint.z < aabb.min.z) {
            temp.updateMinMax(calcPoint);
        } else {
            temp.min.z = calcPoint.z;
        }
    }

    //AABB更新
    mCollider->set(temp.min, temp.max);
}
