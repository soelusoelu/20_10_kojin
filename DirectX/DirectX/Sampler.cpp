#include "Sampler.h"
#include "DirectX.h"
#include "../System/GlobalFunction.h"

Sampler::Sampler(const SamplerDesc& desc) :
    mDesc(desc) {
    DirectX::instance().device()->CreateSamplerState(&toSamplerDesc(desc), &mSampler);
}

Sampler::~Sampler() {
    safeRelease(mSampler);
}

void Sampler::setVSSamplers(unsigned start, unsigned numSamplers) {
    DirectX::instance().deviceContext()->VSSetSamplers(start, numSamplers, &mSampler);
}

void Sampler::setPSSamplers(unsigned start, unsigned numSamplers) {
    DirectX::instance().deviceContext()->PSSetSamplers(start, numSamplers, &mSampler);
}

const SamplerDesc& Sampler::desc() const {
    return mDesc;
}

D3D11_SAMPLER_DESC Sampler::toSamplerDesc(const SamplerDesc& desc) const {
    D3D11_SAMPLER_DESC sd;
    sd.Filter = toFilter(desc.filter);
    sd.AddressU = toAddress(desc.addressU);
    sd.AddressV = toAddress(desc.addressV);
    sd.AddressW = toAddress(desc.addressW);
    sd.MipLODBias = desc.mipLODBias;
    sd.MaxAnisotropy = desc.maxAnisotropy;
    memcpy_s(sd.BorderColor, sizeof(sd.BorderColor), desc.borderColor, sizeof(desc.borderColor));
    sd.MinLOD = desc.minLOD;
    sd.MaxLOD = desc.maxLOD;

    return sd;
}

D3D11_FILTER Sampler::toFilter(Filter filter) const {
    static constexpr D3D11_FILTER filters[]{
        D3D11_FILTER_MIN_MAG_MIP_POINT,
        D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
        D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
        D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
        D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
        D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
        D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
        D3D11_FILTER_MIN_MAG_MIP_LINEAR,
        D3D11_FILTER_ANISOTROPIC,
        D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT,
        D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
        D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
        D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
        D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
        D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
        D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
        D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,
        D3D11_FILTER_COMPARISON_ANISOTROPIC,
        //D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT,
        //D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR,
        //D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
        //D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR,
        //D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT,
        //D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
        //D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT,
        //D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR,
        //D3D11_FILTER_MINIMUM_ANISOTROPIC,
        //D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT,
        //D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR,
        //D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
        //D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR,
        //D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT,
        //D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
        //D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT,
        //D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR,
        //D3D11_FILTER_MAXIMUM_ANISOTROPIC
    };

    return filters[static_cast<unsigned>(filter)];
}

D3D11_TEXTURE_ADDRESS_MODE Sampler::toAddress(TextureAddressMode mode) const {
    static constexpr D3D11_TEXTURE_ADDRESS_MODE modes[]{
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_MIRROR,
        D3D11_TEXTURE_ADDRESS_CLAMP,
        D3D11_TEXTURE_ADDRESS_BORDER,
        D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
    };

    return modes[static_cast<unsigned>(mode)];
}
