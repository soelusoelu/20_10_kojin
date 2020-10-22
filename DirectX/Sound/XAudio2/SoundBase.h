#pragma once

class XAudio2;
class MasteringVoice;

//サウンド最上位管理クラス
class SoundBase {
public:
    SoundBase();
    ~SoundBase();

    /// <summary>
    /// XAudio2を返す
    /// </summary>
    /// <returns></returns>
    XAudio2& getXAudio2() const;

    /// <summary>
    /// マスターボイスを返す
    /// </summary>
    /// <returns></returns>
    MasteringVoice& getMasteringVoice() const;

    /// <summary>
    /// このクラスが使用できるか
    /// </summary>
    /// <returns></returns>
    bool isNull() const;

private:
    SoundBase(const SoundBase&) = delete;
    SoundBase& operator=(const SoundBase&) = delete;

    //COMシステムの初期化
    bool comInitialize();

private:
    //削除順を制御するために生ポインタ
    XAudio2* mXAudio2;
    MasteringVoice* mMasteringVoice;
    bool mSucceedInitialize;

    static inline bool mInstantiated = false;
};
