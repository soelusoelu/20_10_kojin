#pragma once

#include "Sound3DListenerParam.h"
#include "../Sound3DCone.h"
#include "../../../Math/Math.h"

//3Dサウンドで扱うリスナー(聞き手)
class Sound3DListener {
public:
    Sound3DListener();
    ~Sound3DListener();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// リスナーの位置を設定する
    /// </summary>
    /// <param name="pos">位置</param>
    void setPosition(const Vector3& pos);

    /// <summary>
    /// リスナーの前方ベクトルと上ベクトルを設定する
    /// </summary>
    /// <param name="front">前方ベクトル</param>
    /// <param name="top">上ベクトル</param>
    void setFrontAndTop(const Vector3& front, const Vector3& top);

    /// <summary>
    /// コーンを設定する
    /// </summary>
    /// <param name="cone">設定したいコーン</param>
    void setCone(Sound3DCone* cone);

    /// <summary>
    /// リスナーを返す
    /// </summary>
    /// <returns></returns>
    const Sound3DListenerParam& getListener() const;

private:
    Sound3DListener(const Sound3DListener&) = delete;
    Sound3DListener& operator=(const Sound3DListener&) = delete;

private:
    //リスナー
    Sound3DListenerParam mListener;
    //前フレームでの位置
    Vector3 mPreviousPos;

    //リスナーコーン
    static inline Sound3DCone LISTENER_DIRECTIONAL_CONE = { Math::PI * 5.f / 6.f, Math::PI * 11.f / 6.f, 1.f, 0.75f, 0.f, 0.25f, 0.708f, 1.f };
};
