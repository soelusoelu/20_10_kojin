#pragma once

#include "Usage.h"

//バッファタイプ
enum class BufferType {
    BUFFER_TYPE_VERTEX = 0x1L, //頂点バッファ
    BUFFER_TYPE_INDEX = 0x2L, //インデックスバッファ
    BUFFER_TYPE_CONSTANT_BUFFER = 0x4L,
};

enum class BufferCPUAccessFlag {
    CPU_ACCESS_WRITE = 0x10000L,
    CPU_ACCESS_READ = 0x20000L
};

struct BufferDesc {
    //データの一個分のバイト数(本家には無い)
    unsigned oneSize;
    //データのバイト数
    unsigned size;
    //バッファの使用方法
    Usage usage;
    //バッファタイプ(BindFlags)
    unsigned type;
    //cpuアクセス権限
    unsigned cpuAccessFlags;
    //オプション 基本0
    unsigned miscFlags;
    //コンピュートシェーダを使うなら 基本0
    unsigned structureByteStride;

    //コンストラクタ
    BufferDesc() :
        oneSize(0),
        size(0),
        usage(Usage::USAGE_DEFAULT),
        type(static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX)),
        cpuAccessFlags(0),
        miscFlags(0),
        structureByteStride(0) {
    }
};
