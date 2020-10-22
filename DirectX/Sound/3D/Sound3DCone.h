#pragma once

#include "../../Math/Math.h"

//3Dサウンドで扱うコーン構造体
//指向性のある音の大きさを表現する
struct Sound3DCone {
    //ラジアン単位の内部コーン角度 [0.f, 2PI]
    float innerAngle;
    //ラジアン単位の外部コーン角度 [innerAngle, 2PI]
    float outerAngle;
    //内部のボリュームスケーラー
    float innerVolume;
    //外部のボリュームスケーラー
    float outerVolume;
    //LPFダイレクトパスまたはリバーブパスの係数スケーラー [0.f, 1.f]
    float innerLPF;
    //LPFダイレクトパスまたはリバーブパスの係数スケーラー [0.f, 1.f]
    float outerLPF;
    //内部のリバーブセンドレベルスケーラー [0.f, 2.f]
    float innerReverb;
    //外部のリバーブセンドレベルスケーラー [0.f, 2.f]
    float outerReverb;

    //Sound3DCone() :
    //    innerAngle(Math::PI / 2.f),
    //    outerAngle(Math::PI),
    //    innerVolume(1.f),
    //    outerVolume(0.707f),
    //    innerLPF(0.f),
    //    outerLPF(0.25f),
    //    innerReverb(0.707f),
    //    outerReverb(1.f) {
    //}
};
