Texture2D tex : register(t0);
Texture2D normalTex : register(t1);
SamplerState samplerState : register(s0);

cbuffer global_0 : register(b0)
{
    matrix world : packoffset(c0); //ワールド行列
    matrix wvp : packoffset(c4); //ワールドから射影までの変換行列
    float3 lightDir : packoffset(c8); //ライトの方向ベクトル
    float3 cameraPos : packoffset(c9); //カメラ位置
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

    float NL = dot(normal, lightDir);
    float3 Reflect = normalize(2 * NL * normal - lightDir);
    float spec = pow(saturate(dot(Reflect, viewDir)), 4);

    float3 color = saturate(ambient + diffuse.rgb * NL + specular * spec);
    float4 texColor = tex.Sample(samplerState, input.UV);

    color *= texColor.rgb;

    return float4(color, texColor.a * diffuse.a);
}