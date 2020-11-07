cbuffer global_0 : register(b0)
{
    matrix world : packoffset(c0); //���[���h�s��
    matrix wvp : packoffset(c4); //���[���h����ˉe�܂ł̕ϊ��s��
    float3 lightDir : packoffset(c8); //���C�g�̕����x�N�g��
    float3 cameraPos : packoffset(c9); //�J�����ʒu
};

cbuffer global_1 : register(b1)
{
    float3 ambient : packoffset(c0);
    float4 diffuse : packoffset(c1);
    float3 specular : packoffset(c2);
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 WorldPos : POSITION;
};

VS_OUTPUT VS(float3 pos : POSITION, float3 normal : NORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(wvp, float4(pos, 1));
    float3 norm = mul(world, float4(normal, 0)).xyz;
    output.Normal = normalize(norm);
    output.WorldPos = mul(world, float4(pos, 1)).xyz;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 normal = input.Normal;
    float3 viewDir = normalize(cameraPos - input.WorldPos);

    float NL = dot(normal, lightDir);
    float3 Reflect = normalize(2 * NL * normal - lightDir);
    float spec = pow(saturate(dot(Reflect, viewDir)), 4);

    float3 color = saturate(ambient + diffuse.rgb * NL + specular * spec);

    return float4(color, diffuse.a);
}