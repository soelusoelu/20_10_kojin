#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include "../../Math/Math.h"

//サウンドデータ保存クラス
struct SoundData {
    //波形データのサイズ
    const unsigned size;
    //ビットレート
    const unsigned bitRate;
    //1サンプルのバイト数
    const unsigned blockAlign;
    //1秒間のバイト数
    const unsigned averageBytesPerSec;

    SoundData(const ISoundLoader& loader, const WaveFormat& format) :
        size(loader.size()),
        bitRate(format.bitsPerSample),
        blockAlign(format.blockAlign),
        averageBytesPerSec(format.avgBytesPerSec) {
    }

    //秒単位の曲の長さ
    float length() const {
        return (static_cast<float>(size) / static_cast<float>(averageBytesPerSec));
    }

    //曲の範囲内にクランプ
    float clamp(float sec) const {
        //再生時間内にクランプ
        return Math::clamp<float>(sec, 0.f, length());
    }
};
