#include "MeshCommonAndMaterialHeader.hlsli"

Texture2D depthTex : register(t1);

cbuffer Shadow : register(b2)
{
    matrix lightView; //���C�g�̃r���[�s��
};
