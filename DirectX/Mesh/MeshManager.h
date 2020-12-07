#pragma once

#include <list>
#include <memory>

class MeshRenderer;
class ShadowMap;
class Camera;
class DirectionalLight;

class MeshManager {
    using MeshPtr = std::shared_ptr<MeshRenderer>;
    using MeshPtrList = std::list<MeshPtr>;

public:
    MeshManager();
    ~MeshManager();
    void createShadowMap();
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
    bool isDraw(const MeshRenderer& mesh, const Camera& camera) const;
    //メッシュの描画をする
    void drawMeshes(const Camera& camera, const DirectionalLight& dirLight) const;
    //影の描画をする
    void drawShadow(const Camera& camera, const DirectionalLight& dirLight) const;

private:
    MeshPtrList mMeshes;
    std::shared_ptr<ShadowMap> mShadowMap;
};
