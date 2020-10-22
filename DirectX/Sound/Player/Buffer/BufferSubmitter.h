#pragma once

#include "SoundBuffer.h"
#include <xaudio2.h>

class SourceVoice;

//ソースボイスにバッファを登録するクラス
class BufferSubmitter {
public:
    BufferSubmitter(SourceVoice& sourceVoice);
    ~BufferSubmitter();

    /// <summary>
    /// ソースボイスに波形データを追加する
    /// 初めての場合は最初に、既に他のデータが充填されている場合はその次の再生データとして後ろに追加される
    /// 充填できる最大はXAUDIO2_MAX_QUEUED_BUFFERSで64個
    /// </summary>
    /// <param name="buffer">追加するバッファ構造体</param>
    void submitBuffer(const SoundBuffer& buffer) const;

    /// <summary>
    /// ソースボイスに波形データを追加する
    /// 初めての場合は最初に、既に他のデータが充填されている場合はその次の再生データとして後ろに追加される
    /// 充填できる最大はXAUDIO2_MAX_QUEUED_BUFFERSで64個
    /// </summary>
    /// <param name="buffer">追加する簡易版バッファ構造体</param>
    void submitSimpleBuffer(const SimpleSoundBuffer& buffer) const;

private:
    BufferSubmitter(const BufferSubmitter&) = delete;
    BufferSubmitter& operator=(const BufferSubmitter&) = delete;

    //波形データの登録
    void submitSourceBuffer(const XAUDIO2_BUFFER& buffer) const;
    //SoundBufferからXAUDIO2_BUFFERへの変換
    XAUDIO2_BUFFER toBuffer(const SoundBuffer& buffer) const;
    //SimpleSoundBufferからXAUDIO2_BUFFERへの変換
    XAUDIO2_BUFFER toBuffer(const SimpleSoundBuffer& buffer) const;

private:
    SourceVoice& mSourceVoice;
};
