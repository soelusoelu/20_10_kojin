#pragma once

#include <xaudio2.h>

//サウンドエフェクトインターフェース
class ISoundEffect {
public:
    virtual ~ISoundEffect() = default;
    /// <summary>
    /// エフェクト作成純粋仮想関数
    /// </summary>
    /// <param name="desc">XAPO設定用デスクリプタ</param>
    /// <returns>エフェクトの作成に成功したか</returns>
    virtual bool create(XAUDIO2_EFFECT_DESCRIPTOR* desc) = 0;
};
