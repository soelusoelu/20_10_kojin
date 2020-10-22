#pragma once

#include "RasterizerDesc.h"
#include "../System/SystemInclude.h"

class RasterizerState {
public:
    RasterizerState();
    ~RasterizerState();
    void setRasterizerState(const RasterizerDesc& desc);
    //カリング設定
    void setCulling(CullMode mode);
    //ポリゴン塗りつぶし指定
    void setFillMode(FillMode mode);
    const RasterizerDesc& desc() const;

private:
    void execute();

    D3D11_RASTERIZER_DESC toRasterizerDesc(const RasterizerDesc& desc) const;
    D3D11_FILL_MODE toFillMode(const FillMode& mode) const;
    D3D11_CULL_MODE toCullMode(const CullMode& mode) const;

    //コピー禁止
    RasterizerState(const RasterizerState&) = delete;
    RasterizerState& operator=(const RasterizerState&) = delete;

private:
    RasterizerDesc mDesc;
};
