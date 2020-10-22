#pragma once

#include "../Math/Math.h"
#include "ComparisonFunc.h"

enum class Filter {
    FILTER_MIN_MAG_MIP_POINT,
    FILTER_MIN_MAG_POINT_MIP_LINEAR,
    FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
    FILTER_MIN_POINT_MAG_MIP_LINEAR,
    FILTER_MIN_LINEAR_MAG_MIP_POINT,
    FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
    FILTER_MIN_MAG_LINEAR_MIP_POINT,
    FILTER_MIN_MAG_MIP_LINEAR,
    FILTER_ANISOTROPIC,
    FILTER_COMPARISON_MIN_MAG_MIP_POINT,
    FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
    FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
    FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
    FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
    FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
    FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
    FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,
    FILTER_COMPARISON_ANISOTROPIC,
    //FILTER_MINIMUM_MIN_MAG_MIP_POINT,
    //FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR,
    //FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
    //FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR,
    //FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT,
    //FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
    //FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT,
    //FILTER_MINIMUM_MIN_MAG_MIP_LINEAR,
    //FILTER_MINIMUM_ANISOTROPIC,
    //FILTER_MAXIMUM_MIN_MAG_MIP_POINT,
    //FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR,
    //FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
    //FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR,
    //FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT,
    //FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
    //FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT,
    //FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR,
    //FILTER_MAXIMUM_ANISOTROPIC
};

enum class TextureAddressMode {
    WRAP,
    MIRROR,
    CLAMP,
    BORDER,
    MIRROR_ONCE
};

struct SamplerDesc {
    Filter filter;
    TextureAddressMode addressU;
    TextureAddressMode addressV;
    TextureAddressMode addressW;
    float mipLODBias; //計算されたミップマップレベルからのオフセット
    unsigned maxAnisotropy; //FilterでD3D11_FILTER_ANISOTROPICかD3D11_FILTER_COMPARISON_ANISOTROPICが指定されている場合に使用されるクランプ値
    ComparisonFunc comparisonFunc;
    float borderColor[4]; //address○にD3D11_TEXTURE_ADDRESS_BORDERが指定されている場合に使用する境界線の色
    float minLOD; //アクセスをクランプするミップマップ範囲の下限
    float maxLOD; //アクセスをクランプするミップマップ範囲の上限

    SamplerDesc() :
        filter(Filter::FILTER_MIN_MAG_MIP_LINEAR),
        addressU(TextureAddressMode::CLAMP),
        addressV(TextureAddressMode::CLAMP),
        addressW(TextureAddressMode::CLAMP),
        mipLODBias(0.f),
        maxAnisotropy(1),
        comparisonFunc(ComparisonFunc::NEVER),
        minLOD(Math::negInfinity),
        maxLOD(Math::infinity) {
        borderColor[0] = 1.f;
        borderColor[1] = 1.f;
        borderColor[2] = 1.f;
        borderColor[3] = 1.f;
    }
};