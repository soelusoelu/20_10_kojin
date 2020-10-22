Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix mW : packoffset(c0); //ワールド行列
    matrix mWVP : packoffset(c4); //ワールドから射影までの変換行列
};

cbuffer global_1 : register(b1)
{
    float4 mDiffuse : packoffset(c0) = float4(1, 0, 0, 0); //拡散反射(色）
    float4 mSpecular : packoffset(c1) = float4(1, 1, 1, 1); //使わないけど
    float mTexture : packoffset(c2) = 0; //テクスチャーが貼られているメッシュかどうかのフラグ
};

//バーテックスバッファー出力構造体
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : TEXCOORD0;
};

VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    //射影変換（ワールド→ビュー→プロジェクション）
    //法線をワールド空間に
    Norm.w = 0; //移動成分を反映させない
    output.Normal = mul(mW, Norm);
    output.Pos = mul(mWVP, Pos);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return mDiffuse;
}