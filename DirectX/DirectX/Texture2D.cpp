#include "Texture2D.h"
#include "DirectX.h"
#include "Format.h"
#include "Usage.h"
#include "../System/GlobalFunction.h"

Texture2D::Texture2D(const Texture2DDesc& desc) :
    mTexture2D(nullptr),
    mDesc(desc) {
    DirectX::instance().device()->CreateTexture2D(&toTexture2DDesc(desc), nullptr, &mTexture2D);
}

Texture2D::Texture2D(const Texture2DDesc& desc, const SubResourceDesc& data) :
    mTexture2D(nullptr),
    mDesc(desc) {
    DirectX::instance().device()->CreateTexture2D(&toTexture2DDesc(desc), &toSubResource(data), &mTexture2D);
}

Texture2D::Texture2D(ID3D11Texture2D* texture2D) :
    mTexture2D(texture2D),
    mDesc() {
}

Texture2D::~Texture2D() {
    safeRelease(mTexture2D);
}

ID3D11Texture2D* Texture2D::texture2D() const {
    return mTexture2D;
}

const Texture2DDesc& Texture2D::desc() const {
    return mDesc;
}

D3D11_TEXTURE2D_DESC Texture2D::toTexture2DDesc(const Texture2DDesc& desc) const {
    D3D11_TEXTURE2D_DESC td;
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
