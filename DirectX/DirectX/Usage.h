#pragma once

#include "../System/SystemInclude.h"

enum class Usage {
    USAGE_DEFAULT, //GPU�ɂ��ǂݏ���
    USAGE_IMMUTABLE, //GPU�̓ǂݍ��݂̂݉\
    USAGE_DYNAMIC, //GPU�̓ǂݍ��݂�CPU�̏������݂��\
    USAGE_STAGING //GPU����CPU�ւ̃f�[�^�]�����T�|�[�g
};

D3D11_USAGE toUsage(Usage usage);
