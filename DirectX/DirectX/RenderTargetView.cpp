#include "RenderTargetView.h"
#include "DirectX.h"
#include "Format.h"
#include "Texture2D.h"

RenderTargetView::RenderTargetView(const Texture2D& texture2D, const RenderTargetViewDesc* desc) :
    mRenderTargetView(nullptr) {
    auto dev = MyDirectX::DirectX::instance().device();
    if (desc) {
        const auto& temp = toRTVDesc(desc);
        dev->CreateRenderTargetView(texture2D.texture2D(), &temp, &mRenderTargetView);
    } else {
        dev->CreateRenderTargetView(texture2D.texture2D(), nullptr, &mRenderTargetView);
    }
}

RenderTargetView::~RenderTargetView() = default;

void RenderTargetView::setRenderTarget(ID3D11DepthStencilView* depthStencilView) const {
    const auto& dx = MyDirectX::DirectX::instance();
    if (depthStencilView) {
        dx.deviceContext()->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), depthStencilView);
    } else {
        dx.deviceContext()->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), dx.depthStencilView());
    }
}

void RenderTargetView::clearRenderTarget(float r, float g, float b, float a) const {
    const float clearColor[4] = { r, g, b, a };
    MyDirectX::DirectX::instance().deviceContext()->ClearRenderTargetView(mRenderTargetView.Get(), clearColor);
}

void RenderTargetView::setRenderTargets(
    const std::vector<std::unique_ptr<RenderTargetView>>& targets,
    ID3D11DepthStencilView* depthStencilView
) {
    const auto VIEW_COUNT = targets.size();
    std::vector<ID3D11RenderTargetView*> views(VIEW_COUNT);
    for (size_t i = 0; i < VIEW_COUNT; ++i) {
        views[i] = targets[i]->mRenderTargetView.Get();
    }

    const auto& dx = MyDirectX::DirectX::instance();
    if (depthStencilView) {
        dx.deviceContext()->OMSetRenderTargets(VIEW_COUNT, views.data(), depthStencilView);
    } else {
        dx.deviceContext()->OMSetRenderTargets(VIEW_COUNT, views.data(), dx.depthStencilView());
    }
}

D3D11_RENDER_TARGET_VIEW_DESC RenderTargetView::toRTVDesc(const RenderTargetViewDesc* desc) const {
    D3D11_RENDER_TARGET_VIEW_DESC rtvd{};
    rtvd.Format = toFormat(desc->format);
    rtvd.ViewDimension = toDimension(desc->viewDimension);
    rtvd.Texture2D.MipSlice = desc->texture2D.mipSlice;

    return rtvd;
}

D3D11_RTV_DIMENSION RenderTargetView::toDimension(RTVDimension dimension) const {
    static constexpr D3D11_RTV_DIMENSION dimensions[]{
        D3D11_RTV_DIMENSION_TEXTURE2D
    };

    return dimensions[static_cast<unsigned>(dimension)];
}
