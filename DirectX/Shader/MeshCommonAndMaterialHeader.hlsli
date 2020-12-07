#include "MeshCommonHeader.hlsli"

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

cbuffer Material : register(b1)
{
    float3 ambient : packoffset(c0);
    float4 diffuse : packoffset(c1);
    float3 specular : packoffset(c2);
    float shininess : packoffset(c3);
}
