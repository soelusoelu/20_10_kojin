#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Voice/SourceVoice/SourceVoiceInitParam.h"
#include "../Voice/SubmixVoice/SubmixVoiceInitParam.h"
#include <xaudio2.h>
#include <memory>
#include <string>

class SoundBase;
class SourceVoice;
class SubmixVoice;

//サウンド生成クラス
class SoundCreater {
public:
    SoundCreater(const SoundBase& base);
    ~SoundCreater();

    /// <summary>
    /// ソースボイスを作成する
    /// </summary>
    /// <param name="filePath">サウンドが置いてあるファイルパス</param>
    /// <param name="param">初期化用パラメータ</param>
    /// <returns>作成したソースボイス</returns>
    std::shared_ptr<SourceVoice> createSourceVoice(const std::string& filePath, const SourceVoiceInitParam& param) const;

    /// <summary>
    /// サブミックスボイスを作成する
    /// </summary>
    /// <param name="param">初期化用パラメータ</param>
    /// <returns>作成したサブミックスボイス</returns>
    std::shared_ptr<SubmixVoice> createSubmixVoice(const SubmixVoiceInitParam& param) const;

private:
    SoundCreater(const SoundCreater&) = delete;
    SoundCreater& operator=(const SoundCreater&) = delete;

    //拡張子を識別し、パーサーを生成する
    std::unique_ptr<ISoundLoader> createLoaderFromFilePath(const std::string& filePath) const;

private:
    const SoundBase& mSoundBase;
};
