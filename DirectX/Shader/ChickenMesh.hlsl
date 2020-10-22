Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix mW : packoffset(c0); //���[���h�s��
    matrix mWVP : packoffset(c4); //���[���h����ˉe�܂ł̕ϊ��s��
};

cbuffer global_1 : register(b1)
{
    float4 mDiffuse : packoffset(c0) = float4(1, 0, 0, 0); //�g�U����(�F�j
    float4 mSpecular : packoffset(c1) = float4(1, 1, 1, 1); //�g��Ȃ�����
    float mTexture : packoffset(c2) = 0; //�e�N�X�`���[���\���Ă��郁�b�V�����ǂ����̃t���O
};

//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : TEXCOORD0;
};

VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    //�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
    //�@�������[���h��Ԃ�
    Norm.w = 0; //�ړ������𔽉f�����Ȃ�
    output.Normal = mul(mW, Norm);
    output.Pos = mul(mWVP, Pos);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return mDiffuse;
}