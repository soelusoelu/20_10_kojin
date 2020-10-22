#include "DepthStencilState.h"
#include "ComparisonFunc.h"
#include "DirectX.h"

DepthStencilState::DepthStencilState() :
    mDesc() {
    execute();
}

DepthStencilState::~DepthStencilState() = default;

void DepthStencilState::depthTest(bool value) {
    mDesc.depthEnable = value;
    execute();
}

void DepthStencilState::depthMask(bool value) {
    mDesc.depthWriteMask = value;
    execute();
}

void DepthStencilState::stencilTest(bool value) {
    mDesc.stencilEnable = value;
    execute();
}

const DepthStencilDesc& DepthStencilState::desc() const {
    return mDesc;
}

void DepthStencilState::execute() const {
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

    auto& dx = DirectX::instance();
    dx.device()->CreateDepthStencilState(&toDepthStencilDesc(mDesc), &depthStencilState);
    dx.deviceContext()->OMSetDepthStencilState(depthStencilState.Get(), 0);
}

D3D11_DEPTH_STENCIL_DESC DepthStencilState::toDepthStencilDesc(const DepthStencilDesc & desc) const {
    D3D11_DEPTH_STENCIL_DESC dsd;
    dsd.DepthEnable = desc.depthEnable;
    dsd.DepthWriteMask = toDepthWriteMask(desc.depthWriteMask);
    dsd.DepthFunc = toComparisonFunc(desc.depthFunc);
    dsd.StencilEnable = desc.stencilEnable;
    dsd.StencilReadMask = desc.stencilReadMask;
    dsd.StencilWriteMask = desc.stencilWriteMask;
    dsd.FrontFace.StencilFailOp = toStencilOP(desc.frontFace.stencilFailOp);
    dsd.FrontFace.StencilDepthFailOp = toStencilOP(desc.frontFace.stencilDepthFailOp);
    dsd.FrontFace.StencilPassOp = toStencilOP(desc.frontFace.stencilPassOp);
    dsd.FrontFace.StencilFunc = toComparisonFunc(desc.frontFace.stencilFunc);
    dsd.BackFace.StencilFailOp = toStencilOP(desc.backFace.stencilFailOp);
    dsd.BackFace.StencilDepthFailOp = toStencilOP(desc.backFace.stencilDepthFailOp);
    dsd.BackFace.StencilPassOp = toStencilOP(desc.backFace.stencilPassOp);
    dsd.BackFace.StencilFunc = toComparisonFunc(desc.backFace.stencilFunc);

    return dsd;
}

D3D11_DEPTH_WRITE_MASK DepthStencilState::toDepthWriteMask(bool mask) const {
    return (mask) ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
}

D3D11_STENCIL_OP DepthStencilState::toStencilOP(StencilOP stencilOp) const {
    static constexpr D3D11_STENCIL_OP stencilOps[]{
        D3D11_STENCIL_OP_KEEP,
        D3D11_STENCIL_OP_ZERO,
        D3D11_STENCIL_OP_REPLACE,
        D3D11_STENCIL_OP_INCR_SAT,
        D3D11_STENCIL_OP_DECR_SAT,
        D3D11_STENCIL_OP_INVERT,
        D3D11_STENCIL_OP_INCR,
        D3D11_STENCIL_OP_DECR
    };
    return stencilOps[static_cast<unsigned>(stencilOp)];
}
