#pragma once

#include "../DistanceCurve.h"
#include "../Sound3DCone.h"
#include "../../../Math/Math.h"

struct Sound3DEmitterParam {
    //指向性の指定
    //NULLだと無指向(コーン処理が適用されない)
    Sound3DCone* cone;
    //前方ベクトル
    Vector3 orientFront;
    //上ベクトル
    //orientFrontと直行している必要がある
    Vector3 orientTop;

    //エミッターの位置
    Vector3 position;
    //ドップラー計算に使用される速度ベクトル
    Vector3 velocity;

    //内側半径 [0.f, Math::infinity]
    float innerRadius;
    //内半径角度 [0.f, Math::PI / 4.f]
    float innerRadiusAngle;

    //エミッターの数
    unsigned channelCount;
    //channelCountが1より大きい場合にチャンネルが配置される位置からの距離
    float channelRadius;
    //チャンネル半径に沿ったラジアンで方位角として表されるチャンネル位置のテーブル
    float* channelAzimuths;

    //ボリュームレベルの距離曲線
    //NULLだと距離が0.fとcurveDistanceScalerの間にある場合、減衰が適用されないように逆二乗則に準拠する特別なデフォルトカーブが指定される
    DistanceCurve* volumeCurve;
    //LFEロールオフ距離曲線
    //NULLだと[0.f, curveDistanceScaler][curveDistanceScaler, 0.f]
    DistanceCurve* lfeCurve;
    //ローパスフィルター(LPF)直接パス係数距離曲線
    //NULLだと[0.f, 1.f][1.f, 0.75f]
    DistanceCurve* lpfDirectCurve;
    //LPFリバーブパス係数距離カーブ
    //NULLだと[0.f, 0.75f][1.f, 0.75f]
    DistanceCurve* lpfReverbCurve;
    //リバーブセンドレベルの距離カーブ
    //NULLだと[0.f, 1.f][1.f, 0.f]
    DistanceCurve* reverbCurve;

    //正規化された距離曲線をスケーリングしたり、効果を強調したりするために使用される曲線距離スケーラー [Math::negInfinity, Math::infinity]
    float curveDistanceScaler;
    //ドップラーシフト効果を誇張するために使用されるドップラーシフトスケーラー [0.f, Math::infinity]
    float dopplerScaler;

    Sound3DEmitterParam() :
        cone(nullptr),
        orientFront(Vector3::forward),
        orientTop(Vector3::up),
        position(Vector3::zero),
        velocity(Vector3::zero),
        innerRadius(0.f),
        innerRadiusAngle(0.f),
        channelCount(0),
        channelRadius(0.f),
        channelAzimuths(nullptr),
        volumeCurve(nullptr),
        lfeCurve(nullptr),
        lpfDirectCurve(nullptr),
        lpfReverbCurve(nullptr),
        reverbCurve(nullptr),
        curveDistanceScaler(0.f),
        dopplerScaler(0.f) {
    }
};
