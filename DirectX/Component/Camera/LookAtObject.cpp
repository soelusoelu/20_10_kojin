#include "LookAtObject.h"
#include "Camera.h"
#include "../ComponentManager.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

LookAtObject::LookAtObject(GameObject& gameObject) :
    Component(gameObject),
    mCamera(nullptr),
    mTarget(nullptr),
    mCameraPositionOffset(Vector3::zero),
    mLookAtOffset(Vector3::zero)
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
    //設定してあるターゲット + offsetを見る
    mCamera->lookAt(targetTransform.getPosition() + mLookAtOffset);
    //カメラをターゲットの位置 + offsetにする
    mCamera->setPosition(targetTransform.getPosition() + mCameraPositionOffset);
}

void LookAtObject::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getVector3(inObj, "cameraPositionOffset", &mCameraPositionOffset);
    JsonHelper::getVector3(inObj, "lookAtOffset", &mLookAtOffset);
}

void LookAtObject::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("CameraPositionOffset", mCameraPositionOffset);
    inspect->emplace_back("LookAtOffset", mLookAtOffset);
}

void LookAtObject::setTarget(const std::shared_ptr<GameObject>& target) {
    mTarget = target;
}
