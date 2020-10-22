#pragma once

#include <list>
#include <memory>

class MeshComponent;
class Camera;
class DirectionalLight;

class MeshManager {
    using MeshPtr = std::shared_ptr<MeshComponent>;
    using MeshPtrList = std::list<MeshPtr>;

public:
    MeshManager();
    ~MeshManager();
    void update();
    void draw(const Camera& camera, const DirectionalLight& dirLight) const;
    void add(const MeshPtr& mesh);
    void clear();

private:
    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;

    //不要なメッシュを削除する
    void remove();
    //描画するか
    bool isDraw(const MeshComponent& mesh, const Camera& camera) const;

private:
    MeshPtrList mMeshes;
};
