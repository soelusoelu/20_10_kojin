#include "LookAtObject.h"
#include "Camera.h"
#include "../ComponentManager.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"
#include "../../Utility/StringUtil.h"

LookAtObject::LookAtObject(GameObject& gameObject) :
    Component(gameObject),
    mCamera(nullptr),
    mTarget(nullptr),
    mCameraToTargetLength(0.f),
    mCameraPositionOffsetY(0.f),
    mLookAtOffsetY(0.f),
    mLookAtOffsetZ(0.f)
{
}

LookAtObject::~LookAtObject() = default;

void LookAtObject::start() {
    auto cameraObj = gameObject().getGameObjectManager().find("Camera");
    mCamera = cameraObj->componentManager().getComponent<Camera>();
}

void LookAtObject::update() {
    //ターゲットが設定されていないのはおかしい
    if (!mTarget) {
        Debug::logError("Not set target.");
        return;
    }

    const auto& targetTransform = mTarget->transform();
    //ターゲット + offsetを見る
    auto offset = targetTransform.forward() * mLookAtOffsetZ + Vector3::up * mLookAtOffsetY;
    mCamera->lookAt(targetTransform.getPosition() + offset);
    //カメラをターゲットの位置 + offsetにする
    offset = targetTransform.forward() * -mCameraToTargetLength + Vector3::up * mCameraPositionOffsetY;
    mCamera->setPosition(targetTransform.getPosition() + offset);
}

void LookAtObject::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "cameraToTargetLenght", &mCameraToTargetLength);
    JsonHelper::getFloat(inObj, "cameraPositionOffsetY", &mCameraPositionOffsetY);
    JsonHelper::getFloat(inObj, "lookAtOffsetY", &mLookAtOffsetY);
    JsonHelper::getFloat(inObj, "lookAtOffsetZ", &mLookAtOffsetZ);
}

void LookAtObject::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("CameraToTargetLenght", mCameraToTargetLength);
    inspect->emplace_back("CameraPositionOffsetY", mCameraPositionOffsetY);
    inspect->emplace_back("LookAtOffsetY", mLookAtOffsetY);
    inspect->emplace_back("LookAtOffsetZ", mLookAtOffsetZ);
}

void LookAtObject::setTarget(const std::shared_ptr<GameObject>& target) {
    mTarget = target;
}
