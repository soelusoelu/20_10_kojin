#include "FrameHeader.h"

FrameHeader::FrameHeader() :
    mVersion(0),
    mLayer(0),
    mBitRate(0),
    mSampleRate(0),
    mPadding(0),
    mChannels(0),
    mFrameSize(0) {
}

FrameHeader::~FrameHeader() = default;

bool FrameHeader::isFrameHeader(const BYTE* header) {
    //先頭バイトは0xFF
    if ((header[0] & 0xFF) != 0xFF) {
        return false;
    }
    //次のバイトの上位3ビットは1
    if ((header[1] & 0xE0) != 0xE0) {
        return false;
    }

    //フレームヘッダである
    return true;
}

bool FrameHeader::parseFrameHeader(const BYTE* header) {
    //MPEGのバージョンの取得
    mVersion = 4 - ((header[1] >> 3) & 0x03);
    if (mVersion == 3) {
        return false;
    }
    if (mVersion == 4) {
        mVersion = 3;
    }

    //レイヤー数の取得
    mLayer = 4 - ((header[1] >> 1) & 0x03);
    if (mLayer == 4) {
        return false;
    }

    //ビットレートの取得
    int index = header[2] >> 4;
    mBitRate = BIT_RATE_TABLE[(mVersion == 1) ? 0 : 1][mLayer - 1][index];
    if (mBitRate == -1) {
        return false;
    }

    //サンプリングレートの取得
    index = (header[2] >> 2) & 0x03;
    mSampleRate = SAMPLE_RATE_TABLE[mVersion - 1][index];
    if (mSampleRate == -1) {
        return false;
    }

    //パディングの取得
    mPadding = (header[2] >> 1) & 0x01;
    //チャンネル数の取得
    mChannels = ((header[3] >> 6) == 3) ? 1 : 2;
    //フレームサイズの取得
    mFrameSize = ((SAMPLE_PER_FRAME[mLayer - 1][mVersion - 1] / 8 * mBitRate * 1000) / mSampleRate) + mPadding;
    //mFrameSize = ((144 * mBitRate * 1000) / mSampleRate) + mPadding;

    //強調モードが予約コードの場合は終了
    if ((header[3] & 0x03) == 2) {
        return false;
    }

    return true;
}

void FrameHeader::setDataToMpegLayer3WaveFormat(MPEGLAYER3WAVEFORMAT* format) {
    //WAVEFORMATEX部分
    format->wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
    format->wfx.nChannels = mChannels;
    format->wfx.nSamplesPerSec = mSampleRate;
    format->wfx.nAvgBytesPerSec = (mBitRate * 1000) / 8;
    format->wfx.nBlockAlign = 1;
    format->wfx.wBitsPerSample = 0;
    format->wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;

    //MPEGLAYER3WAVEFORMATオリジナル部分
    format->wID = MPEGLAYER3_ID_MPEG;
    format->fdwFlags = (mPadding) ? MPEGLAYER3_FLAG_PADDING_ON : MPEGLAYER3_FLAG_PADDING_OFF;
    format->nBlockSize = mFrameSize;
    format->nFramesPerBlock = 1;
    format->nCodecDelay = 1393;
}
