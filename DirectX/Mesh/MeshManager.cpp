#include "MeshManager.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Mesh/MeshComponent.h"
#include "../DirectX/DirectXInclude.h"
#include "../Transform/Transform3D.h"

MeshManager::MeshManager() {
    MeshComponent::setMeshManager(this);
}

MeshManager::~MeshManager() {
    MeshComponent::setMeshManager(nullptr);
}

void MeshManager::update() {
    remove();
}

void MeshManager::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    if (mMeshes.empty()) {
        return;
    }

    for (const auto& mesh : mMeshes) {
        if (!isDraw(*mesh, camera)) {
            continue;
        }

        //MyDirectX::DirectX::instance().rasterizerState()->setCulling(CullMode::FRONT);
        //mesh->draw(camera, dirLight);

        MyDirectX::DirectX::instance().rasterizerState()->setCulling(CullMode::BACK);
        mesh->draw(camera, dirLight);
    }
}

void MeshManager::add(const MeshPtr& mesh) {
    mMeshes.emplace_back(mesh);
}

void MeshManager::clear() {
    mMeshes.clear();
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

bool MeshManager::isDraw(const MeshComponent& mesh, const Camera& camera) const {
    if (!mesh.getActive()) {
        return false;
    }
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
