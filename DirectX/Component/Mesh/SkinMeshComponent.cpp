#include "SkinMeshComponent.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../../DebugLayer/Debug.h"
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
    ++mCurrentFrame;
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
        meshcb.bones[i] = bones[i].offsetMat * bones[i].frameMat[mCurrentFrame];
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

    for (size_t i = 0; i < bones.size(); i++) {
        if (bones[i].parent) {
            Debug::renderLine(
                Vector3::transform(Vector3::zero, bones[i].frameMat[mCurrentFrame]),
                Vector3::transform(Vector3::zero, bones[i].parent->frameMat[mCurrentFrame])
            );
        }
    }
}
