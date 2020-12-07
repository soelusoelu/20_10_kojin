cbuffer Common : register(b0)
{
    matrix world : packoffset(c0); //ワールド行列
    matrix view : packoffset(c4); //ビュー行列
    matrix projection : packoffset(c8); //プロジェクション行列
    matrix wvp : packoffset(c12); //ワールドから射影までの変換行列
    float3 lightDir : packoffset(c16); //ライトの方向ベクトル
    float3 lightColor : packoffset(c17); //ライトの色
    float3 cameraPos : packoffset(c18); //カメラ位置
}
