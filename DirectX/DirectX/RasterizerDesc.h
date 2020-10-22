#pragma once

enum class FillMode {
    WIREFRAME, //頂点を結ぶ線を引く
    SOLID //ポリゴンを塗りつぶす
};

enum class CullMode {
    NONE, //常にすべてのポリゴンを描画
    FRONT, //前向きのポリゴンを描画しない
    BACK //後ろ向きのポリゴンを描画しない
};

struct RasterizerDesc {
    //レンダリング時の塗りつぶしモード
    FillMode fillMode;
    //指定された方向を向いたポリゴン描画されない
    CullMode cullMode;
    //頂点が時計回りのとき、ポリゴンは正面と見なされ、反時計回りのとき、背面と見なされる
    bool frontCounterClockwise;
    //特定のピクセルに追加される深度値
    int depthBias;
    //ピクセルの最大深度バイアス
    float depthBiasClamp;
    //指定されたピクセルの勾配のスカラー
    float slopeScaledDepthBias;
    //距離に基づいたクリップの有効化
    bool depthClipEnable;
    bool scissorEnable;
    bool multisampleEnable;
    bool antialiasedLineEnable;

    RasterizerDesc() :
        fillMode(FillMode::SOLID),
        cullMode(CullMode::BACK),
        frontCounterClockwise(false),
        depthBias(0),
        depthBiasClamp(0.f),
        slopeScaledDepthBias(0.f),
        depthClipEnable(true),
        scissorEnable(false),
        multisampleEnable(false),
        antialiasedLineEnable(false) {
    }
};
