#pragma once

#include <xaudio2.h>

class SourceVoice;
class SoundPlayer;

//ループを扱うクラス
class SoundLoop {
public:
    SoundLoop(SourceVoice& sourceVoice, SoundPlayer& player);
    ~SoundLoop();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// ループ区間を指定する
    /// </summary>
    /// <param name="begin">ループ開始地点(単位: 秒数)</param>
    /// <param name="end">ループ折返し地点(単位: 秒数) 0なら曲の終わりまで</param>
    void setLoopPoint(float begin, float end = 0.f);

    /// <summary>
    /// 曲の最初から最後までループ
    /// </summary>
    void loopAll();

    /// <summary>
    /// ループから抜ける
    /// </summary>
    void exitLoop();

    /// <summary>
    /// ループしている最中か
    /// </summary>
    /// <returns></returns>
    bool isLoop() const;

    /// <summary>
    /// ループの終わりを返す
    /// </summary>
    /// <returns></returns>
    float getLoopEnd() const;

private:
    SoundLoop(const SoundLoop&) = delete;
    SoundLoop& operator=(const SoundLoop&) = delete;

    //ループ設定共通関数
    void setValue(float begin, float end);
    //再生時間内にクランプ
    void clamp();

private:
    SourceVoice& mSourceVoice;
    SoundPlayer& mPlayer;
    //ループ開始地点 sec
    float mLoopBegin;
    //ループ折返し地点 sec
    float mLoopEnd;
    //ループ中か
    bool mIsLoop;
};
