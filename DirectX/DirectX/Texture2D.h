#pragma once

#include "SubResourceDesc.h"
#include "Texture2DDesc.h"
#include "../System/SystemInclude.h"

class Texture2D {
public:
    Texture2D(const Texture2DDesc& desc);
    Texture2D(const Texture2DDesc& desc, const SubResourceDesc& data);
    Texture2D(ID3D11Texture2D* texture2D);
    ~Texture2D();
    ID3D11Texture2D* texture2D() const;
    const Texture2DDesc& desc() const;

private:
    D3D11_TEXTURE2D_DESC toTexture2DDesc(const Texture2DDesc& desc) const;

    //コピー禁止
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

private:
    ID3D11Texture2D* mTexture2D;
    Texture2DDesc mDesc;
};
