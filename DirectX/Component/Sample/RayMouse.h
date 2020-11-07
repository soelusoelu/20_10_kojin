#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include <memory>

class Camera;
class MeshComponent;
class AABBCollider;

class RayMouse : public Component {
public:
    RayMouse(GameObject& gameObject);
    ~RayMouse();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<MeshComponent> mMesh;
    std::shared_ptr<AABBCollider> mAABB;
    Vector3 mIntersectPoint;
    bool mSelectedMesh;
};
