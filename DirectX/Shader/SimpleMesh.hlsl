cbuffer global_0
{
    matrix wvp; //ƒ[ƒ‹ƒh‚©‚çË‰e‚Ü‚Å‚Ì•ÏŠ·s—ñ
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(wvp, pos);
    output.Normal = normal;
    output.UV = uv;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(input.Normal, 1);
}