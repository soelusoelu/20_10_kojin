#pragma once

#include "Listener/Sound3DListenerParam.h"
#include "../../Device/Flag.h"
#include <xaudio2.h>
#include <x3daudio.h>

class MasteringVoice;
class Sound3DEmitter;

//3Dサウンド計算クラス
class Sound3DCalculator {
public:
    Sound3DCalculator(const MasteringVoice& masteringVoice);
    ~Sound3DCalculator();

    /// <summary>
    /// 3Dサウンドを計算する
    /// </summary>
    /// <param name="listenerParam">リスナー構造体</param>
    /// <param name="emitter">エミッター</param>
    void calculate(const Sound3DListenerParam& listener, const Sound3DEmitter& emitter) const;

private:
    Sound3DCalculator(const Sound3DCalculator&) = delete;
    Sound3DCalculator& operator=(const Sound3DCalculator&) = delete;

    //フラグを設定し返す
    Flag getFlagsFromEmitter(const Sound3DEmitter& emitter) const;

private:
    //3Dサウンドのハンドル
    X3DAUDIO_HANDLE mInstance;
    //初期化に成功しているか
    bool mInitialized;
};
