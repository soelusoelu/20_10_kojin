#pragma once

#include "Format.h"

enum class RTVDimension {
    RTV_DIMENSION_TEXTURE2D
};

struct Texture2DRenderTargetView {
    unsigned mipSlice;

    Texture2DRenderTargetView() :
        mipSlice(0) {
    }
};

struct RenderTargetViewDesc {
    Format format;
    RTVDimension viewDimension;
    Texture2DRenderTargetView texture2D;

    RenderTargetViewDesc() :
        format(Format::FORMAT_RGBA8_UNORM),
        viewDimension(RTVDimension::RTV_DIMENSION_TEXTURE2D),
        texture2D() {
    }
};
