#pragma once

#include "BlendDesc.h"
#include "../System/SystemInclude.h"

class BlendState {
public:
    BlendState();
    ~BlendState();
    void setBlendState(const BlendDesc& desc, unsigned renderTarget = 0);
    //通常合成
    void normal(unsigned renderTarget = 0);
    //加算合成
    void add(unsigned renderTarget = 0);
    //減算合成
    void subtraction(unsigned renderTarget = 0);
    //色反転
    void reverseColor(unsigned renderTarget = 0);
    //半透明合成
    void translucent(unsigned renderTarget = 0);
    //レンダーターゲットごとに設定を反映するか
    void independentBlendEnable(bool value);
    //デスクの取得
    const BlendDesc& desc() const;

private:
    void execute(unsigned renderTarget) const;

    D3D11_BLEND_DESC toBlendDesc(const BlendDesc& desc, unsigned renderTarget) const;
    D3D11_BLEND toBlend(const Blend& blend) const;
    D3D11_BLEND_OP toBlendOP(const BlendOP& blendOp) const;
    unsigned toColorWriteEnable(const ColorWriteEnable& color) const;

    //コピー禁止
    BlendState(const BlendState&) = delete;
    BlendState& operator=(const BlendState&) = delete;

private:
    BlendDesc mDesc;
};
