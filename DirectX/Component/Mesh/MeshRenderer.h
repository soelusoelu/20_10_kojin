#pragma once

#include "IDrawBefore.h"
#include "../Component.h"
#include <memory>

class MeshComponent;
class MeshShader;
class ShadowMap;
class Camera;
class DirectionalLight;
class MeshManager;

//メッシュ描画専用クラス
class MeshRenderer : public Component, public std::enable_shared_from_this<MeshRenderer> {
public:
    MeshRenderer(GameObject& gameObject);
    ~MeshRenderer();
    virtual void start() override;

    //描画
    void draw(const Camera& camera, const DirectionalLight& dirLight) const;
    //描画前描画をしたいクラスを登録する
    void setDrawBefore(const IDrawBefore* drawer);
    //メッシュコンポーネントを取得する
    const MeshComponent& getMeshComponent() const;
    //メッシュシェーダーを取得する
    MeshShader& getMeshShader() const;

    //自身を管理するマネージャーを登録する
    static void setMeshManager(MeshManager* manager);

private:
    MeshRenderer(const MeshRenderer&) = delete;
    MeshRenderer& operator=(const MeshRenderer&) = delete;

    //メインメッシュ描画
    void drawMesh(const Camera& camera, const DirectionalLight& dirLight) const;
    //スキンメッシュコンポーネントを条件に合えば作る
    void createSkinMeshComponent();
    //マネージャーに登録する
    void addToManager();

private:
    const IDrawBefore* mBeforeDrawer;
    std::shared_ptr<MeshComponent> mMeshComponent;
    std::shared_ptr<MeshShader> mMeshShader;
    std::shared_ptr<ShadowMap> mShadowMap;

    static inline MeshManager* mMeshManager = nullptr;
};
