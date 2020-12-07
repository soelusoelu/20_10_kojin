#pragma once

#include "ShaderResourceViewDesc.h"
#include "../System/SystemInclude.h"
#include <memory>

class Texture2D;

class ShaderResourceView {
public:
    ShaderResourceView(const Texture2D& texture2D, const ShaderResourceViewDesc* desc = nullptr);
    ShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view);
    ~ShaderResourceView();
    void setVSShaderResources(unsigned start = 0, unsigned numViews = 1) const;
    void setPSShaderResources(unsigned start = 0, unsigned numViews = 1) const;

private:
    D3D11_SHADER_RESOURCE_VIEW_DESC toSRVDesc(const ShaderResourceViewDesc& desc) const;
    D3D11_SRV_DIMENSION toDimension(SRVDimension dimension) const;

    ShaderResourceView(const ShaderResourceView&) = delete;
    ShaderResourceView& operator=(const ShaderResourceView&) = delete;

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
};
