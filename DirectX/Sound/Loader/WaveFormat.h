#pragma once

#include <xaudio2.h>

//すべての非PCMフォーマットに使用される拡張波形フォーマット構造
struct WaveFormat {
    //フォーマットタイプ
    unsigned short formatTag;
    //チャンネル数
    unsigned short channels;
    //サンプリングレート
    unsigned long samplesPerSec;
    //バッファ推定用
    unsigned long avgBytesPerSec;
    //データのブロックサイズ
    unsigned short blockAlign;
    //ビットレート
    unsigned short bitsPerSample;
    //よくわからん
    unsigned short cbSize;

    WaveFormat() :
        formatTag(0),
        channels(0),
        samplesPerSec(0),
        avgBytesPerSec(0),
        blockAlign(0),
        bitsPerSample(0),
        cbSize(0) {
    }

    WaveFormat(const WAVEFORMATEX& format) :
        formatTag(format.wFormatTag),
        channels(format.nChannels),
        samplesPerSec(format.nSamplesPerSec),
        avgBytesPerSec(format.nAvgBytesPerSec),
        blockAlign(format.nBlockAlign),
        bitsPerSample(format.wBitsPerSample),
        cbSize(format.cbSize) {
    }
};
