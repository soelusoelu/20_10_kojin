#pragma once

#include "Collider.h"
#include "../../Math/Math.h"
#include "../../Utility/Collision.h"
#include <memory>

class SphereCollider : public Collider {
public:
    SphereCollider(GameObject& gameObject);
    ~SphereCollider();
    virtual void start() override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    virtual void onUpdateWorldTransform() override;
    void set(const Vector3& center, float radius);
    const Sphere& getSphere() const;

private:
    std::unique_ptr<Sphere> mSphere;
    Vector3 mDefaultCenter;
    float mDefaultRadius;
};
