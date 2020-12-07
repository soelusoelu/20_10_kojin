#include "Texture2D.h"
#include "DirectX.h"
#include "Format.h"
#include "Usage.h"

Texture2D::Texture2D(const Texture2DDesc& desc, const SubResourceDesc* data) :
    mTexture2D(nullptr),
    mDesc(desc) {
    const auto& temp = toTexture2DDesc(desc);
    if (data) {
        const auto& sub = toSubResource(*data);
        MyDirectX::DirectX::instance().device()->CreateTexture2D(&temp, &sub, &mTexture2D);
    } else {
        MyDirectX::DirectX::instance().device()->CreateTexture2D(&temp, nullptr, &mTexture2D);
    }
}

Texture2D::Texture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D) :
    mTexture2D(texture2D),
    mDesc() {
}

Texture2D::~Texture2D() = default;

ID3D11Texture2D* Texture2D::texture2D() const {
    return mTexture2D.Get();
}

const Texture2DDesc& Texture2D::desc() const {
    return mDesc;
}

D3D11_TEXTURE2D_DESC Texture2D::toTexture2DDesc(const Texture2DDesc& desc) const {
    D3D11_TEXTURE2D_DESC td{};
    td.Width = desc.width;
    td.Height = desc.height;
    td.MipLevels = desc.mipLevels;
    td.ArraySize = desc.arraySize;
    td.Format = toFormat(desc.format);
    td.SampleDesc.Count = desc.sampleDesc.count;
    td.SampleDesc.Quality = desc.sampleDesc.quality;
    td.Usage = toUsage(desc.usage);
    td.BindFlags = desc.bindFlags;
    td.CPUAccessFlags = desc.cpuAccessFlags;
    td.MiscFlags = desc.miscFlags;

    return td;
}
