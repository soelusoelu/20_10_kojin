#pragma once

//頂点タイプ
enum class VertexType {
    VERTEX_TYPE_FLOAT1,
    VERTEX_TYPE_FLOAT2,
    VERTEX_TYPE_FLOAT3,
    VERTEX_TYPE_FLOAT4,
    VERTEX_TYPE_BYTE4,
    VERTEX_TYPE_BYTE4N,
    VERTEX_TYPE_UBYTE4,
    VERTEX_TYPE_UBYTE4N,
    VERTEX_TYPE_SHORT2,
    VERTEX_TYPE_SHORT2N,
    VERTEX_TYPE_USHORT2,
    VERTEX_TYPE_USHORT2N,
    VERTEX_TYPE_SHORT4,
    VERTEX_TYPE_SHORT4N,
    VERTEX_TYPE_USHORT4,
    VERTEX_TYPE_USHORT4N,
    VERTEX_TYPE_INT1,
    VERTEX_TYPE_INT2,
    VERTEX_TYPE_INT3,
    VERTEX_TYPE_INT4,
    VERTEX_TYPE_UINT1,
    VERTEX_TYPE_UINT2,
    VERTEX_TYPE_UINT3,
    VERTEX_TYPE_UINT4
};

//頂点か否か
enum class SlotClass {
    SLOT_CLASS_VERTEX_DATA,
    SLOT_CLASS_INSTANCE_DATA
};

//頂点宣言ディスクリプタ
struct InputElementDesc {
    //セマンティック LPCSTR
    const char* semantic;
    //セマンティックインデックス
    unsigned index;
    //頂点タイプ
    VertexType type;
    //スロット
    unsigned slot;
    //オフセット
    unsigned offset;
    //頂点か否か
    SlotClass classification;
    //ストリーム番号
    unsigned stream;
};

//頂点宣言の最大値
const unsigned VERTEX_DECLARATION_MAX = 16;
