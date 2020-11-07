#pragma once

#include "../Voice/SourceVoice/SourceVoiceInitParam.h"
#include "../Voice/SubmixVoice/SubmixVoiceInitParam.h"
#include <memory>
#include <string>

class SoundBase;
class SourceVoice;
class SubmixVoice;
class SoundCreater;
class SoundManager;
class Sound3DListener;

class SoundEngine {
private:
    SoundEngine();
public:
    ~SoundEngine();

    /// <summary>
    /// サウンドエンジンのインスタンスを返す
    /// </summary>
    /// <returns>インスタンス</returns>
    static SoundEngine& instance();

    /// <summary>
    /// 終了処理
    /// </summary>
    void finalize();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// 3D演算で扱うリスナーを登録する
    /// </summary>
    /// <param name="listener">登録したいリスナー</param>
    void setListener(const std::shared_ptr<Sound3DListener>& listener);

    std::shared_ptr<SourceVoice> createSourceVoice(const std::string& fileName, const SourceVoiceInitParam& param, const std::string& directoryPath = "Assets\\Sound\\") const;
    std::shared_ptr<SubmixVoice> createSubmixVoice(const SubmixVoiceInitParam& param) const;

    const SoundBase& getBase() const;

private:
    SoundEngine(const SoundEngine&) = delete;
    SoundEngine& operator=(const SoundEngine&) = delete;
    SoundEngine(SoundEngine&&) = delete;
    SoundEngine& operator=(SoundEngine&&) = delete;

private:
    std::unique_ptr<SoundBase> mBase;
    std::unique_ptr<SoundCreater> mCreater;
    std::unique_ptr<SoundManager> mManager;

    static inline SoundEngine* mInstance = nullptr;
};
