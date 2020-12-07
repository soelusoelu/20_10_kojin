cbuffer OutLine : register(b0)
{
    matrix wvp; //ワールドから射影までの変換行列
    float4 outLineColor; //アウトラインの色
}
