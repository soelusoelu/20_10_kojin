#pragma once

#include "SamplerDesc.h"
#include "../System/SystemInclude.h"

class Sampler {
public:
    Sampler(const SamplerDesc& desc);
    ~Sampler();
    void setVSSamplers(unsigned start = 0, unsigned numSamplers = 1);
    void setPSSamplers(unsigned start = 0, unsigned numSamplers = 1);
    const SamplerDesc& desc() const;

private:
    D3D11_SAMPLER_DESC toSamplerDesc(const SamplerDesc& desc) const;
    D3D11_FILTER toFilter(Filter filter) const;
    D3D11_TEXTURE_ADDRESS_MODE toAddress(TextureAddressMode mode) const;

private:
    ID3D11SamplerState* mSampler;
    SamplerDesc mDesc;
};
