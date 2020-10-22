#pragma once

//PlayTimerエフェクトのパラメータ
struct PlayTimerParam {
    //設定したい時間(単位: 秒)
    float setTime;
    //再生速度(ピッチ)
    float frequencyRatio;

    PlayTimerParam() :
        setTime(0.f),
        frequencyRatio(1.f) {
    }
};
