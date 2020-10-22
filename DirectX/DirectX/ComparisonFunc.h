#pragma once

#include "../System/SystemInclude.h"

//�V�����f�[�^��ۑ�����O�ɁA�����^�C�����V�����f�[�^�������̃f�[�^�Ɣ�r������@
enum class ComparisonFunc {
    NEVER, //��r���Ȃ�
    LESS, //src < dest -> pass
    EQUAL, //src == dest -> pass
    LESS_EQUAL, //src <= dest -> pass
    GREATER, //src > dest -> pass
    NOT_EQUAL, //src != dest -> pass
    GREATER_EQUAL, //src >= dest -> pass
    ALWAYS //��ɐ���
};

D3D11_COMPARISON_FUNC toComparisonFunc(ComparisonFunc func);
