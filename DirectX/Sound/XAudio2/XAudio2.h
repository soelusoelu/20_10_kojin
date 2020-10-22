#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Voice/SourceVoice/SourceVoiceInitParam.h"
#include "../Voice/SubmixVoice/SubmixVoiceInitParam.h"
#include "../../System/SystemInclude.h"
#include <xaudio2.h>
#include <memory>

class MasteringVoice;
class SourceVoice;
class SubmixVoice;

//IXAudio2ラッパークラス
class XAudio2 {
public:
    XAudio2(bool* successFlag);
    ~XAudio2();

    /// <summary>
    /// マスターボイスを作成する
    /// </summary>
    /// <returns>作成したマスターボイス</returns>
    MasteringVoice* createMasteringVoice() const;

    /// <summary>
    /// ソースボイスを作成する
    /// </summary>
    /// <param name="masteringVoice">マスターボイス</param>
    /// <param name="data">事前に取得してあるサウンドデータ</param>
    /// <param name="param">初期化用パラメータ</param>
    /// <returns>作成したソースボイス</returns>
    std::unique_ptr<SourceVoice> createSourceVoice(MasteringVoice& masteringVoice, std::unique_ptr<ISoundLoader>& loader, const WAVEFORMATEX& format, const SourceVoiceInitParam& param) const;

    /// <summary>
    /// サブミックスボイスを作成する
    /// </summary>
    /// <param name="masteringVoice">マスターボイス</param>
    /// <param name="param">初期化用パラメータ</param>
    /// <returns>作成したサブミックスボイス</returns>
    std::unique_ptr<SubmixVoice> createSubmixVoice(MasteringVoice& masteringVoice, const SubmixVoiceInitParam& param) const;

private:
    XAudio2(const XAudio2&) = delete;
    XAudio2& operator=(const XAudio2&) = delete;

    //XAudio2の生成
    bool createXAudio2();

private:
    Microsoft::WRL::ComPtr<IXAudio2> mXAudio2;

    static inline bool mInstantiated = false;
};
