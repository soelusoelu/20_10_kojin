Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);
Texture2D g_texSpecular : register(t3);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix world; //���[���h�s��
    matrix wvp; //���[���h����ˉe�܂ł̕ϊ��s��
};

cbuffer global_1 : register(b1)
{
    float3 ambient : packoffset(c0);
    float4 diffuse : packoffset(c1);
    float3 specular : packoffset(c2);
}

//�o�[�e�b�N�X�o�b�t�@�[�o��
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float3 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
};

//�s�N�Z���V�F�[�_�[�o��
struct PS_OUTPUT
{
    float4 Color : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Specular : SV_Target3;
};

VS_OUTPUT VS(float4 Pos : POSITION, float3 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = mul(wvp, Pos);
    output.WorldPos = mul(world, Pos);
    output.WorldNormal = mul(world, float4(Norm, 1)).xyz;

    output.UV = UV;

    return output;
}

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;

    //�J���[�e�N�X�`���[�֏o��
    Out.Color = diffuse;
    Out.Color = g_texColor.Sample(g_samLinear, input.UV);

    //���[���h�@���e�N�X�`���[�֏o��
    float3 normal = input.WorldNormal;
    Out.Normal = float4(normal * 0.5 + 0.5, 1.0);

    //���[���h���W�e�N�X�`���[�֏o��
    Out.Position = float4(input.WorldPos.xyz, 1.0);

    //�X�y�L�����e�N�X�`���֏o��
    Out.Specular = float4(specular, 1.0);

    return Out;
}
