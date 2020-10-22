#pragma once

#include "../Component.h"
#include "../../Math/Math.h"

class Camera;

class LookAtObject : public Component {
public:
    LookAtObject(GameObject& gameObject);
    ~LookAtObject();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void setTarget(const std::shared_ptr<GameObject>& target);

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<GameObject> mTarget;
    float mCameraToTargetLength;
    float mCameraPositionOffsetY;
    float mLookAtOffsetY;
    float mLookAtOffsetZ;
};
