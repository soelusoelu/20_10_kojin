cbuffer global
{
    matrix wp;
    float4 color;
};

//ç\ë¢ëÃ
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};

VS_OUTPUT VS(float4 Pos : POSITION)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(wp, Pos);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return color;
}