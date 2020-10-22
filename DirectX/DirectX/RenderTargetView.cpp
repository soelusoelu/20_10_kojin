#include "RenderTargetView.h"
#include "DirectX.h"
#include "Format.h"
#include "Texture2D.h"

RenderTargetView::RenderTargetView(const Texture2D& texture2D, const RenderTargetViewDesc* desc) :
    mRenderTargetView(nullptr) {
    auto dev = DirectX::instance().device();
    if (desc) {
        dev->CreateRenderTargetView(texture2D.texture2D(), &toRTVDesc(desc), &mRenderTargetView);
    } else {
        dev->CreateRenderTargetView(texture2D.texture2D(), nullptr, &mRenderTargetView);
    }
}

RenderTargetView::~RenderTargetView() = default;

ID3D11RenderTargetView* RenderTargetView::getRenderTarget() const {
    return mRenderTargetView.Get();
}

void RenderTargetView::clearRenderTarget(float r, float g, float b, float a) const {
    const float clearColor[4] = { r, g, b, a };
    DirectX::instance().deviceContext()->ClearRenderTargetView(mRenderTargetView.Get(), clearColor);
}

D3D11_RENDER_TARGET_VIEW_DESC RenderTargetView::toRTVDesc(const RenderTargetViewDesc* desc) const {
    D3D11_RENDER_TARGET_VIEW_DESC rtvd;
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
