#pragma once

#include "MeshComponent.h"
#include "../../Math/Math.h"
#include "../../Mesh/Bone.h"

class SkinMeshComponent : public MeshComponent {
public:
    SkinMeshComponent(GameObject& gameObject);
    ~SkinMeshComponent();
    virtual void update() override;
    virtual void draw(const Camera& camera, const DirectionalLight& dirLight) const override;
    void test(Bone& me, const Matrix4* parentWorld) const;

private:
    int mCurrentFrame;
    std::vector<Bone> mBones;
};
