#include "SimpleHeader.hlsli"

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Depth : TEXCOORD0;
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(wvp, pos);
    //テクスチャ座標を頂点に合わせる
    output.Depth = output.Pos;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    //Z値算出
    return input.Depth.z / input.Depth.w;
}