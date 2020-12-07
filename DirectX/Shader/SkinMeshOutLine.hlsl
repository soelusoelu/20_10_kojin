#include "OutLineHeader.hlsli"

cbuffer Animation : register(b1)
{
    matrix bones[256];
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, float4 weight : BLENDWEIGHT, min16uint4 index : BLENDINDICES)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    matrix comb = (float4x4) 0;
    for (int i = 0; i < 4; ++i)
    {
        comb += bones[index[i]] * weight[i];
    }

    float4 position = mul(comb, pos);
    output.Pos = mul(wvp, position);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return outLineColor;
}