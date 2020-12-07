cbuffer Common : register(b0)
{
    matrix world : packoffset(c0); //���[���h�s��
    matrix view : packoffset(c4); //�r���[�s��
    matrix projection : packoffset(c8); //�v���W�F�N�V�����s��
    matrix wvp : packoffset(c12); //���[���h����ˉe�܂ł̕ϊ��s��
    float3 lightDir : packoffset(c16); //���C�g�̕����x�N�g��
    float3 lightColor : packoffset(c17); //���C�g�̐F
    float3 cameraPos : packoffset(c18); //�J�����ʒu
}
