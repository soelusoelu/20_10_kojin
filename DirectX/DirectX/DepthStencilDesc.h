#pragma once

#include "ComparisonFunc.h"
#include "../System/SystemInclude.h"

//深度ステンシルテスト中に実行できるステンシル操作
enum class StencilOP {
    KEEP, //既存のステンシルデータを保持
    ZERO, //ステンシルデータを0に設定
    REPLACE, //ID3D11DeviceContext::OMSetDepthStencilStateを呼び出して、ステンシルデータを参照値セットに設定
    INCR_SAT, //ステンシル値を1増やし、クランプ
    DECR_SAT, //ステンシル値を1減らし、クランプ
    INVERT, //データ反転
    INCR, //ステンシル値を1増やし、必要に応じてラップ
    DECR //ステンシル値を1減らし、必要に応じてラップ
};

struct DepthStencilOPDesc {
    //ステンシルテストが失敗したときのステンシル操作
    StencilOP stencilFailOp;
    //ステンシルテストに合格し、深度テストが失敗したときのステンシル操作
    StencilOP stencilDepthFailOp;
    //ステンシルテストと深度テストの両方に合格したときのステンシル操作
    StencilOP stencilPassOp;
    //ステンシルデータを既存のステンシルデータと比較
    ComparisonFunc stencilFunc;
};

struct DepthStencilDesc {
    //深度テスト有効化指定
    bool depthEnable;
    //深度データを書き込むための深度ステンシルバッファの部分を識別
    bool depthWriteMask;
    //比較オプション
    ComparisonFunc depthFunc;
    //ステンシルテスト有効化指定
    bool stencilEnable;
    //ステンシルデータを読み取るための深度ステンシルバッファの一部を識別
    unsigned stencilReadMask;
    //ステンシルデータを書き込むための深度ステンシルバッファの一部を識別
    unsigned stencilWriteMask;
    //表面法線がカメラに向いているピクセルの深度テストとステンシルテストの結果の使用方法
    DepthStencilOPDesc frontFace;
    //深度テストとステンシルテストの結果を使用して、表面の法線がカメラとは反対の方向に向いているピクセルの使用方法
    DepthStencilOPDesc backFace;

    DepthStencilDesc() :
        depthEnable(true),
        depthWriteMask(true),
        depthFunc(ComparisonFunc::LESS),
        stencilEnable(false),
        stencilReadMask(D3D11_DEFAULT_STENCIL_READ_MASK),
        stencilWriteMask(D3D11_DEFAULT_STENCIL_WRITE_MASK) {
        frontFace.stencilFailOp = StencilOP::KEEP;
        backFace.stencilFailOp = StencilOP::KEEP;
        frontFace.stencilDepthFailOp = StencilOP::KEEP;
        backFace.stencilDepthFailOp = StencilOP::KEEP;
        frontFace.stencilPassOp = StencilOP::KEEP;
        backFace.stencilPassOp = StencilOP::KEEP;
        frontFace.stencilFunc = ComparisonFunc::ALWAYS;
        backFace.stencilFunc = ComparisonFunc::ALWAYS;
    }
};
