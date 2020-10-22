#pragma once

#include "DepthStencilDesc.h"
#include "../System/SystemInclude.h"

class DepthStencilState {
public:
    DepthStencilState();
    ~DepthStencilState();
    void depthTest(bool value);
    void depthMask(bool value);
    void stencilTest(bool value);
    const DepthStencilDesc& desc() const;

private:
    void execute() const;

    D3D11_DEPTH_STENCIL_DESC toDepthStencilDesc(const DepthStencilDesc& desc) const;
    D3D11_DEPTH_WRITE_MASK toDepthWriteMask(bool mask) const;
    D3D11_STENCIL_OP toStencilOP(StencilOP stencilOp) const;

    //コピー禁止
    DepthStencilState(const DepthStencilState&) = delete;
    DepthStencilState& operator=(const DepthStencilState&) = delete;

private:
    DepthStencilDesc mDesc;
};
