//グローバル
Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global
{
    matrix wvp;
    float4 color;
    float4 uv;
};

//構造体
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

VS_OUTPUT VS(float4 Pos : POSITION, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(wvp, Pos);
    output.UV = UV * float2(uv.z - uv.x, uv.w - uv.y) + float2(uv.x, uv.y);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 col = g_texDecal.Sample(g_samLinear, input.UV);
    col *= color;
    return col;
}