#include "MeshManager.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Mesh/MeshRenderer.h"
#include "../Component/Mesh/ShadowMap.h"
#include "../DirectX/DirectXInclude.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Transform/Transform3D.h"

MeshManager::MeshManager()
    : mShadowMap(nullptr)
{
    MeshRenderer::setMeshManager(this);
}

MeshManager::~MeshManager() {
    MeshRenderer::setMeshManager(nullptr);
}

void MeshManager::createShadowMap() {
    auto sm = GameObjectCreater::create("ShadowMap");
    mShadowMap = sm->componentManager().getComponent<ShadowMap>();
}

void MeshManager::update() {
    remove();
}

void MeshManager::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    if (mMeshes.empty()) {
        return;
    }

    MyDirectX::DirectX::instance().rasterizerState()->setCulling(CullMode::BACK);

    if (mShadowMap) {
        drawShadow(camera, dirLight);
    }

    drawMeshes(camera, dirLight);
}

void MeshManager::add(const MeshPtr& mesh) {
    mMeshes.emplace_back(mesh);
}

void MeshManager::clear() {
    remove();
}

void MeshManager::remove() {
    auto itr = mMeshes.begin();
    while (itr != mMeshes.end()) {
        if ((*itr)->isDead()) {
            itr = mMeshes.erase(itr);
        } else {
            ++itr;
        }
    }
}

bool MeshManager::isDraw(const MeshRenderer& mesh, const Camera& camera) const {
    if (mesh.isDead()) {
        return false;
    }
    //auto pos = mesh.transform().getPosition();
    //auto radius = mesh.getMesh().getRadius() * mesh.transform().getScale().x;
    //if (!camera.viewFrustumCulling(pos, radius)) {
    //    return false;
    //}

    return true;
}

void MeshManager::drawMeshes(const Camera& camera, const DirectionalLight& dirLight) const {
    for (const auto& mesh : mMeshes) {
        if (!isDraw(*mesh, camera)) {
            continue;
        }

        mesh->draw(camera, dirLight);
    }
}

void MeshManager::drawShadow(const Camera& camera, const DirectionalLight& dirLight) const {
    //描画準備
    mShadowMap->drawBegin(dirLight);

    for (const auto& mesh : mMeshes) {
        //描画できないなら次へ
        if (!isDraw(*mesh, camera)) {
            continue;
        }

        //描画
        mShadowMap->draw(*mesh, camera, dirLight);

        //影描画用の定数バッファを設定する
        mShadowMap->setShadowConstantBuffer(*mesh);
    }

    //描画終了処理
    mShadowMap->drawEnd();
}
