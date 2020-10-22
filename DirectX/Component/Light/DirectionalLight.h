#pragma once

#include "../Component.h"
#include "../../Math/Math.h"

class DirectionalLight : public Component {
public:
    DirectionalLight(GameObject& gameObject);
    ~DirectionalLight();
    virtual void onUpdateWorldTransform() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void setDirection(const Vector3& dir);
    const Vector3& getDirection() const;
    void setLightColor(const Vector3& color);
    const Vector3& getLightColor() const;

private:
    Vector3 mDirection;
    Vector3 mLightColor;
};
