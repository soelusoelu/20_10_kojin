#pragma once

#include "../../../Device/Flag.h"
#include <xaudio2.h>

//ソースボイス作成時の初期化パラメータ
struct SourceVoiceInitParam {
    //ボイスに属性を設定するフラグ
    Flag flags;
    //ボイスの最大許容再生速度を設定する
    //最大XAUDIO2_MAX_FREQ_RATIO(1024倍)まで可能
    float maxFrequencyRatio;
    //コールバック
    IXAudio2VoiceCallback* callback;
    //ボイスの出力先を設定する
    //nullだとマスターボイス直結になる
    //後からsetOutputVoicesで設定可能
    const XAUDIO2_VOICE_SENDS* sendList;
    //エフェクト指定
    //nullだと使用しない
    //後からsetEffectChainsで設定可能
    const XAUDIO2_EFFECT_CHAIN* effectChain;

    //3D演算をするか
    bool isCalculate3D;

    SourceVoiceInitParam() :
        flags(),
        maxFrequencyRatio(XAUDIO2_DEFAULT_FREQ_RATIO),
        callback(nullptr),
        sendList(nullptr),
        effectChain(nullptr),
        isCalculate3D(false) {
    }
};
