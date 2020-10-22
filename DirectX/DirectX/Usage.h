#pragma once

#include "../System/SystemInclude.h"

enum class Usage {
    USAGE_DEFAULT, //GPUによる読み書き
    USAGE_IMMUTABLE, //GPUの読み込みのみ可能
    USAGE_DYNAMIC, //GPUの読み込みとCPUの書き込みが可能
    USAGE_STAGING //GPUからCPUへのデータ転送をサポート
};

D3D11_USAGE toUsage(Usage usage);
