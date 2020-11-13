#include "SkinMeshComponent.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../../Device/Time.h"
#include "../../Mesh/Material.h"
#include "../../Mesh/Mesh.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../System/Shader/Shader.h"
#include "../../System/Texture/TextureFromFile.h"
#include "../../Transform/Transform3D.h"

SkinMeshComponent::SkinMeshComponent(GameObject& gameObject) :
    MeshComponent(gameObject),
    mCurrentFrame(0)
{
}

SkinMeshComponent::~SkinMeshComponent() = default;

void SkinMeshComponent::update() {
    auto& bones = mMesh->getBones();
    mBones.resize(bones.size());
    for (size_t i = 0; i < bones.size(); i++) {
        mBones[i].offsetMat = bones[i].frameMat[mCurrentFrame];
        mBones[i].initMat = bones[i].frameMat[mCurrentFrame];
    }
    test(mBones[0], nullptr);
    ++mCurrentFrame;;
    if (mCurrentFrame >= bones[0].numFrame) {
        mCurrentFrame = 0;
    }
}

void SkinMeshComponent::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    //使用するシェーダーの登録
    mShader->setShaderInfo();

    //シェーダーのコンスタントバッファーに各種データを渡す
    SkinMeshConstantBuffer meshcb;
    auto& bones = mMesh->getBones();
    for (size_t i = 0; i < bones.size(); i++) {
        //meshcb.bones[i] = Matrix4::identity;
        //meshcb.bones[i] = bones[i].initMat;
        //meshcb.bones[i] = bones[i].offsetMat * bones[i].initMat;
        //meshcb.bones[i] = bones[i].offsetMat * Matrix4::inverse(bones[i].offsetMat);
        meshcb.bones[i] = bones[i].offsetMat * bones[i].frameMat[mCurrentFrame];
        //meshcb.bones[i] = bones[i].offsetMat * bones[i].frameMat[mCurrentFrame] * Matrix4::inverse(bones[i].offsetMat);
        //meshcb.bones[i] = Matrix4::inverse(bones[i].initMat) * bones[i].frameMat[mCurrentFrame] * bones[i].initMat;
        //meshcb.bones[i] = bones[i].offsetMat * bones[i].frameMat[mCurrentFrame] * bones[i].initMat;
        //meshcb.bones[i] = bones[i].offsetMat * mBones[i].initMat;
        //meshcb.bones[i] = bones[i].offsetMat * bones[i].frameMat[mCurrentFrame] * mBones[i].initMat;
    }
    const auto& world = transform().getWorldTransform();
    meshcb.world = world;
    meshcb.wvp = world * camera.getViewProjection();
    meshcb.lightDir = dirLight.getDirection();
    meshcb.cameraPos = camera.getPosition();
    mShader->transferData(&meshcb, sizeof(meshcb), 0);

    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        MaterialConstantBuffer matcb;
        const auto& mat = mMesh->getMaterial(i);
        matcb.ambient = mat.ambient;
        //アルファ値は0のときが多いから
        float alpha = mAlpha;
        if (!Math::nearZero(mat.transparency)) {
            alpha *= mat.transparency;
        }
        matcb.diffuse = Vector4(mat.diffuse, alpha);
        matcb.specular = mat.specular;
        //データ転送
        mShader->transferData(&matcb, sizeof(matcb), 1);

        //テクスチャが有るなら登録
        if (mat.texture) {
            mat.texture->setTextureInfo();
        }

        //描画
        mMesh->draw(i);
    }
}

void SkinMeshComponent::test(Bone& me, const Matrix4* parentWorld) const {
    if (parentWorld) {
        me.initMat *= *parentWorld;
    }
    for (size_t i = 0; i < me.children.size(); i++) {
        test(*me.children[i], &me.offsetMat);
    }
}
