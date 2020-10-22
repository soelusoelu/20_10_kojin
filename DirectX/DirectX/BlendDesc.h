#pragma once

enum class Blend {
    ZERO,
    ONE,
    SRC_COLOR,
    INV_SRC_COLOR,
    SRC_ALPHA,
    INV_SRC_ALPHA,
    DEST_ALPHA,
    INV_DEST_ALPHA,
    DEST_COLOR,
    INV_DEST_COLOR,
    SRC_ALPHA_SAT,
    BLEND_FACTOR,
    INV_BLEND_FACTOR,
    SRC1_COLOR,
    INV_SRC1_COLOR,
    SRC1_ALPHA,
    INV_SRC1_ALPHA
};

enum class BlendOP {
    ADD, //1 + 2
    SUBTRACT, //2 - 1
    REV_SUBTRACT, //1 - 2
    MIN, //min(1, 2)
    MAX //max(1, 2)
};

enum class ColorWriteEnable {
    RED,
    GREEN,
    BLUE,
    ALPHA,
    ALL
};

struct RenderTargetBlendDesc {
    //ブレンドの有効化指定
    bool blendEnable;
    //ブレンドの仕方
    Blend srcBlend;
    Blend destBlend;
    BlendOP blendOp;
    //アルファも
    Blend srcBlendAlpha;
    Blend destBlendAlpha;
    BlendOP blendOpAlpha;
    //書き込みマスク
    ColorWriteEnable renderTargetWriteMask;
};

struct BlendDesc {
    bool alphaToCoverageEnable;
    //レンダーターゲットごとに設定するか
    //trueだとごとに falseだとrenderTarget[0]が共通使用
    bool independentBlendEnable;
    //各種レンダーターゲット 最大8
    RenderTargetBlendDesc renderTarget;

    BlendDesc() :
        alphaToCoverageEnable(false),
        independentBlendEnable(false) {
        renderTarget.blendEnable = true;
        renderTarget.srcBlend = Blend::SRC_ALPHA;
        renderTarget.destBlend = Blend::INV_SRC_ALPHA;
        renderTarget.blendOp = BlendOP::ADD;
        renderTarget.srcBlendAlpha = Blend::ONE;
        renderTarget.destBlendAlpha = Blend::ZERO;
        renderTarget.blendOpAlpha = BlendOP::ADD;
        renderTarget.renderTargetWriteMask = ColorWriteEnable::ALL;
    }
};
