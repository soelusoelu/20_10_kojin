#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include <memory>

class SourceVoice;
class SoundPlayer;
class BufferSubmitter;

//ストリーミングを扱うクラス
class SoundStreaming {
public:
    SoundStreaming(SourceVoice& sourceVoice, SoundPlayer& player, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format);
    ~SoundStreaming();

    //バッファの状態を監視する
    void polling();

    /// <summary>
    /// 指定したファイル位置までシークする
    /// </summary>
    /// <param name="point">ファイルの先頭からの位置(単位: 秒数)</param>
    void seek(float point);

private:
    SoundStreaming(const SoundStreaming&) = delete;
    SoundStreaming& operator=(const SoundStreaming&) = delete;

    //バッファを追加する
    void addBuffer();
    //セカンダリバッファに指定サイズ分バッファを読み込む
    long read(unsigned readSize);
    //初期状態に戻す
    void initialize();
    //remainBufferSizeを再計算する
    void recomputeRemainBufferSize(unsigned point);

private:
    static constexpr unsigned BUFFER_COUNT = 2;
    static constexpr unsigned BUFFER_QUEUE_MAX = 2;

    SourceVoice& mSourceVoice;
    SoundPlayer& mPlayer;
    std::unique_ptr<BufferSubmitter> mBufferSubmitter;
    std::unique_ptr<ISoundLoader> mLoader;
    //バッファ
    BYTE* mBuffer[BUFFER_COUNT];
    //バッファ番号
    unsigned mPrimary;
    unsigned mSecondary;
    //ファイルから読み込むサイズ
    const unsigned READ_SIZE;
    //バッファの余り
    unsigned mRemainBufferSize;
    //読み込みカーソル
    unsigned mWrite;
    //ファイルの終端まで読み込んだか
    bool mEndOfFile;
};
