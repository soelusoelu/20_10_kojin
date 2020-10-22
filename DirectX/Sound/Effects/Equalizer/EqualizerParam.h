#pragma once

//イコライザーパラメータ
struct EqualizerParam {
    //中心周波数(Hz)
    //最低20Hz ～ 最高20KHz
    float frequencyCenter[4];
    //それぞれの中心周波数地点での音量
    //単位は倍率、1.fが元の音量
    //最低0.126f ～ 最高7.94fまで
    float gain[4];
    //バンド幅
    float bandWidth[4];
};
