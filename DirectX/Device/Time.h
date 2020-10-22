#pragma once

class Time {
public:
    Time(float sec = 0.f);
    ~Time();
    void update();
    //設定を最初の状態に戻す
    void reset();
    //カウントアップタイムがリミットタイムを超えたか
    bool isTime() const;
    //リミットタイムの再設定
    void setLimitTime(float sec);
    //カウントアップタイムの強制設定
    void setCurrentTime(float sec);
    //現在のカウントアップタイムを返す
    float countUpTime() const;
    //現在のカウントダウンタイムを返す
    float countDownTime() const;
    //リミットタイムを返す
    float limitTime() const;
    //リミットタイムに対してのカウントアップタイムの比率
    float rate() const;
    //現在時刻をミリ秒で返す
    static unsigned long long time();

public:
    static inline float deltaTime = 0.f;

private:
    float mTimeRate;
    float mLimitTime;
    bool mIsOverLimit;
};
