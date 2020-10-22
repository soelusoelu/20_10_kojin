#pragma once

#include "Format.h"

enum class SRVDimension {
    SRV_DIMENSION_TEXTURE2D
};

struct Texture2DShaderResourceView {
    unsigned mostDetailedMip;
    unsigned mipLevels;

    Texture2DShaderResourceView() :
        mostDetailedMip(0),
        mipLevels(1) {
    }
};

struct ShaderResourceViewDesc {
    Format format;
    SRVDimension viewDimension;
    Texture2DShaderResourceView texture2D;

    ShaderResourceViewDesc() :
        format(Format::FORMAT_RGBA8_UNORM),
        viewDimension(SRVDimension::SRV_DIMENSION_TEXTURE2D),
        texture2D() {
    }
};
