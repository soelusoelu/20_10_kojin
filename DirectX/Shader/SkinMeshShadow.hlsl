#include "ShadowHeader.hlsli"

cbuffer Animation : register(b3)
{
    matrix bones[256];
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
    float3 WorldPos : POSITION;
    float4 ZCalcTex : TEXCOORD1; //Z�l�Z�o�p�e�N�X�`��
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
    float3 norm = mul(world, float4(normal, 0)).xyz;
    output.Normal = normalize(norm);
    output.UV = uv;
    output.WorldPos = mul(world, position).xyz;

    matrix mat = mul(lightView, world);
    mat = mul(projection, mat);
    output.ZCalcTex = mul(mat, pos);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 normal = input.Normal;
    float3 viewDir = normalize(cameraPos - input.WorldPos);
    float4 zCalcTex = input.ZCalcTex;

    float NL = dot(normal, -lightDir);

    //�g�U���ˌ�
    float3 diff = NL * diffuse.rgb;

    //���ˌ��x�N�g��
    float3 reflect = normalize(lightDir + 2 * NL * normal);
    //���ʔ��ˌ�
    float spec = pow(saturate(dot(reflect, viewDir)), shininess) * specular;

    float3 color = saturate(ambient + diff + spec) * lightColor;
    float4 texColor = tex.Sample(samplerState, input.UV);

    float4 outColor = float4(color * texColor.rgb, diffuse.a * texColor.a);

    //���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
    float ZValue = zCalcTex.z / zCalcTex.w;

    //�e�N�X�`�����W�ɕϊ�
    float2 transTexCoord;
    transTexCoord.x = (1.0 + zCalcTex.x / zCalcTex.w) * 0.5;
    transTexCoord.y = (1.0 - zCalcTex.y / zCalcTex.w) * 0.5;

    // �������W��Z�l�𒊏o
    float SM_Z = depthTex.Sample(samplerState, transTexCoord).x;

    //�Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
    if (ZValue > SM_Z + 0.005)
    {
        outColor.rgb *= 0.5;
    }

    return outColor;
}