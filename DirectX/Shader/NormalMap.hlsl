#include "MeshCommonAndMaterialHeader.hlsli"

Texture2D normalTex : register(t1);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
    float3 Tangent : TANGENT;
    float3 WorldPos : POSITION;
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, float3 tangent : TANGENT)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(wvp, pos);
    float3 norm = mul(world, float4(normal, 0)).xyz;
    output.Normal = normalize(norm);
    output.UV = uv;
    output.Tangent = tangent;
    output.WorldPos = mul(world, pos).xyz;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 normal = input.Normal;
    float3 viewDir = normalize(cameraPos - input.WorldPos);

    float NL = dot(normal, -lightDir);

    //ŠgŽU”½ŽËŒõ
    float3 diff = NL * diffuse.rgb;

    //”½ŽËŒõƒxƒNƒgƒ‹
    float3 reflect = normalize(lightDir + 2 * NL * normal);
    //‹¾–Ê”½ŽËŒõ
    float spec = pow(saturate(dot(reflect, viewDir)), shininess) * specular;

    float3 color = saturate(ambient + diff + spec) * lightColor;
    float4 texColor = tex.Sample(samplerState, input.UV);

    return float4(color * texColor.rgb, diffuse.a * texColor.a);
}