#pragma once

#include "../../../Device/Flag.h"
#include <xaudio2.h>

//サブミックスボイス作成時の初期化パラメータ
struct SubmixVoiceInitParam {
    //ボイスのチャンネル数
    unsigned channels;
    //ボイスのサンプリング周波数
    unsigned inputSampleRate;
    //ボイスに属性を設定するフラグ
    Flag flags;
    //複数のサブミックスボイスが存在するときの処理順番の数値
    //複雑なルーティングを組んで処理順番が音に影響するようなときに使用する
    //数字が小さい方から順に処理が行われる
    unsigned processingStage;
    //ボイスの出力先を設定する
    //nullだとマスターボイス直結になる
    //後からsetOutputVoicesで設定可能
    const XAUDIO2_VOICE_SENDS* sendList;
    //エフェクト指定
    //nullだと使用しない
    //後からsetEffectChainsで設定可能
    const XAUDIO2_EFFECT_CHAIN* effectChain;

    SubmixVoiceInitParam() :
        channels(0),
        inputSampleRate(0),
        flags(),
        processingStage(0),
        sendList(nullptr),
        effectChain(nullptr) {
    }
};
