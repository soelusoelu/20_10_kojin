#pragma once

#include "../../System/SystemInclude.h"
#include <mmreg.h>

class FrameHeader {
public:
    FrameHeader();
    ~FrameHeader();

    //フレームヘッダか
    bool isFrameHeader(const BYTE* header);
    //フレームヘッダの解析
    bool parseFrameHeader(const BYTE* header);
    //MPEGLAYER3WAVEFORMATにデータを詰め込む
    void setDataToMpegLayer3WaveFormat(MPEGLAYER3WAVEFORMAT* format);

private:
    FrameHeader(const FrameHeader&) = delete;
    FrameHeader& operator=(const FrameHeader&) = delete;

public:
    //フレームヘッダのサイズ
    static constexpr unsigned FRAME_HEADER_SIZE = 4;

private:
    //MPEGのバージョン
    int mVersion;
    //レイヤー数
    int mLayer;
    //ビットレート
    int mBitRate;
    //サンプリングレート
    int mSampleRate;
    //パディング
    int mPadding;
    //チャンネル数
    int mChannels;
    //フレームサイズ
    int mFrameSize;

    //ビットレートテーブル
    static constexpr int BIT_RATE_TABLE[2][3][16] = {
        {
            { 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, -1 }, //MPEG1-Layer1
            { 0, 32, 48, 56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, -1 }, //MPEG1-Layer2
            { 0, 32, 40, 48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, -1 }  //MPEG1-Layer3
        }, {
            { 0, 32, 48, 56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256, -1 }, //MPEG2/2.5-Layer1
            { 0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, -1 }, //MPEG2/2.5-Layer2
            { 0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, -1 }  //MPEG2/2.5-Layer3
        }
    };

    //サンプリングレートテーブル
    static constexpr int SAMPLE_RATE_TABLE[3][4] = {
        { 44100, 48000, 32000, -1 }, //MPGEG1
        { 22050, 24000, 16000, -1 }, //MPEG2
        { 11025, 12000,  8000, -1 }  //MPEG2.5
    };

    //フレーム当たりのサンプル数テーブル
    static constexpr int SAMPLE_PER_FRAME[3][4] = {
        {  384,  384,  384, -1 }, //layer1
        { 1152, 1152, 1152, -1 }, //layer2
        { 1152,  576,  576, -1 }  //layer3
    };
};
