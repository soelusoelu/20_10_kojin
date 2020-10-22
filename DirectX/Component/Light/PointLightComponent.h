#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include <memory>

class Camera;
class LightManager;
struct PointLight;

class PointLightComponent : public Component, public std::enable_shared_from_this<PointLightComponent> {
public:
    PointLightComponent(GameObject& gameObject);
    ~PointLightComponent();
    virtual void start() override;
    virtual void finalize() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void draw(const Camera& camera, const PointLight& pointLight) const;
    void setLightColor(const Vector3& color);
    void setInnerRadius(float radius);
    void setOuterRadius(float radius);
    void setIntensity(float value);

    static void setLightManager(LightManager* manager);

private:
    Vector3 mLightColor; //色
    float mInnerRadius; //この半径以内だと完全な輝度で照らす
    float mOuterRadius; //光の届く半径
    float mIntensity; //光の強度

    static inline LightManager* mLightManager = nullptr;
};
