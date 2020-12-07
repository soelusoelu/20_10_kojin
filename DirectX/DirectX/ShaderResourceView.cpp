#include "ShaderResourceView.h"
#include "DirectX.h"
#include "Format.h"
#include "Texture2D.h"

ShaderResourceView::ShaderResourceView(const Texture2D& texture2D, const ShaderResourceViewDesc* desc) :
    mShaderResourceView(nullptr) {
    if (desc) {
        const auto& temp = toSRVDesc(*desc);
        MyDirectX::DirectX::instance().device()->CreateShaderResourceView(texture2D.texture2D(), &temp, &mShaderResourceView);
    } else {
        MyDirectX::DirectX::instance().device()->CreateShaderResourceView(texture2D.texture2D(), nullptr, &mShaderResourceView);
    }
}

ShaderResourceView::ShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view) :
    mShaderResourceView(view) {
}

ShaderResourceView::~ShaderResourceView() = default;

void ShaderResourceView::setVSShaderResources(unsigned start, unsigned numViews) const {
    MyDirectX::DirectX::instance().deviceContext()->VSSetShaderResources(start, numViews, mShaderResourceView.GetAddressOf());
}

void ShaderResourceView::setPSShaderResources(unsigned start, unsigned numViews) const {
    MyDirectX::DirectX::instance().deviceContext()->PSSetShaderResources(start, numViews, mShaderResourceView.GetAddressOf());
}

D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceView::toSRVDesc(const ShaderResourceViewDesc& desc) const {
    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    srvd.Format = toFormat(desc.format);
    srvd.ViewDimension = toDimension(desc.viewDimension);
    srvd.Texture2D.MostDetailedMip = desc.texture2D.mostDetailedMip;
    srvd.Texture2D.MipLevels = desc.texture2D.mipLevels;

    return srvd;
}

D3D11_SRV_DIMENSION ShaderResourceView::toDimension(SRVDimension dimension) const {
    static constexpr D3D11_SRV_DIMENSION dimensions[]{
        D3D11_SRV_DIMENSION_TEXTURE2D
    };

    return dimensions[static_cast<unsigned>(dimension)];
}
