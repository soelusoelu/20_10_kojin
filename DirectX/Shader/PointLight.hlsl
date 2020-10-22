Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);
Texture2D g_texSpecular : register(t3);

SamplerState g_samLinear : register(s0);

cbuffer PointLight : register(b0)
{
    matrix mWVP : packoffset(c0);
    float3 mWorldPos : packoffset(c4);
    float3 mCameraPos : packoffset(c5);
    float2 mWindowSize : packoffset(c6);
    float3 mDiffuseColor : packoffset(c7);
    float mInnerRadius : packoffset(c8);
    float mOuterRadius : packoffset(c9);
    float mIntensity : packoffset(c10);
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float3 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
};

VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = mul(mWVP, Pos);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float2 gbufferCoord = input.Pos.xy / mWindowSize;

    //テクスチャーから情報を取り出す
    float3 gbufferDiffuse = g_texColor.Sample(g_samLinear, gbufferCoord).xyz;
    float3 gbufferWorldNormal = g_texNormal.Sample(g_samLinear, gbufferCoord).xyz;
    float3 gbufferWorldPos = g_texPosition.Sample(g_samLinear, gbufferCoord).xyz;
    float3 gbufferSpecular = g_texSpecular.Sample(g_samLinear, gbufferCoord).xyz;

    float3 N = normalize(gbufferWorldNormal);
    float3 L = normalize(mWorldPos - gbufferWorldPos);
    float3 V = normalize(mCameraPos - gbufferWorldPos);
    float3 R = normalize(reflect(-L, N));

    float3 Phong = float3(0.0, 0.0, 0.0);
    float NdotL = dot(N, L);
    if (NdotL > 0)
    {
        float dist = distance(mWorldPos, gbufferWorldPos);
        //smoothstep min以下は0 max以上は1 間は補間
        float intensity = smoothstep(mInnerRadius, mOuterRadius, dist);
        float3 DiffuseColor = lerp(mDiffuseColor, float3(0.0, 0.0, 0.0), intensity);
        float3 Specular = pow(max(0.0, dot(R, V)), 4.0) * gbufferSpecular;
        Phong = (DiffuseColor + Specular) * NdotL;

        Phong *= mIntensity;
        
        Phong = saturate(Phong);
    }

    return float4(gbufferDiffuse * Phong, 1.0);
}
