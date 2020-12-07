#pragma once

#include "RenderTargetViewDesc.h"
#include "../System/SystemInclude.h"
#include <memory>
#include <vector>

class Texture2D;

class RenderTargetView {
public:
    RenderTargetView(const Texture2D& texture2D, const RenderTargetViewDesc* desc = nullptr);
    ~RenderTargetView();
    //レンダーターゲットを設定する
    void setRenderTarget(ID3D11DepthStencilView* depthStencilView = nullptr) const;
    //レンダーターゲットをクリアする
    void clearRenderTarget(float r = 0.f, float g = 0.f, float b = 1.f, float a = 1.f) const;

    //複数のレンダーターゲットを設定する
    static void setRenderTargets(
        const std::vector<std::unique_ptr<RenderTargetView>>& targets,
        ID3D11DepthStencilView* depthStencilView = nullptr
    );

private:
    D3D11_RENDER_TARGET_VIEW_DESC toRTVDesc(const RenderTargetViewDesc* desc) const;
    D3D11_RTV_DIMENSION toDimension(RTVDimension dimension) const;

private:
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
};
