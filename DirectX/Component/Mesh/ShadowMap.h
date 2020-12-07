#pragma once

#include "../Component.h"
#include "../../DirectX/Texture2DDesc.h"
#include "../../Math/Math.h"
#include "../../System/SystemInclude.h"
#include "../../System/Shader/ConstantBuffers.h"
#include <memory>

class Shader;
class Texture2D;
class RenderTargetView;
class ShaderResourceView;
class MeshRenderer;
class Camera;
class DirectionalLight;

//モデルの影を落とすクラス
class ShadowMap : public Component {
public:
    ShadowMap(GameObject& gameObject);
    ~ShadowMap();
    virtual void awake() override;

    //描画準備
    void drawBegin(const DirectionalLight& dirLight);
    //描画
    void draw(const MeshRenderer& renderer, const Camera& camera, const DirectionalLight& dirLight) const;
    //影描画に使用するコンスタントバッファを登録する
    void setShadowConstantBuffer(MeshRenderer& renderer);
    //描画終了処理
    void drawEnd() const;
    //深度を書き込んだテクスチャをGPUに送る
    void transferShadowTexture(unsigned constantBufferIndex = 1);

private:
    ShadowMap(const ShadowMap&) = delete;
    ShadowMap& operator=(const ShadowMap&) = delete;

    //深度テクスチャ用ディスクリプタを作成する
    void createDepthDesc(Texture2DDesc& desc) const;
    //深度テクスチャを作成する
    void createDepthTexture(const Texture2DDesc& desc);
    //深度テクスチャ用レンダーターゲットビューを作成する
    void createDepthRenderTargetView(Format format);
    //深度テクスチャ用深度ステンシルビューを作成する
    void createDepthStencilView(const Texture2DDesc& desc);
    //深度テクスチャ用シェーダーリソースビューを作成する
    void createDepthShaderResourceView(Format format);

private:
    std::shared_ptr<Shader> mDepthTextureCreateShader;
    std::unique_ptr<Texture2D> mDepthTexture;
    std::unique_ptr<RenderTargetView> mDepthRenderTargetView;
    std::shared_ptr<ShaderResourceView> mDepthShaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
    ShadowConstantBuffer mShadowConstBuffer;
    int mWidth;
    int mHeight;
};
