#pragma once

#include "Bone.h"
#include "Motion.h"
#include <string>

//外部公開用アニメーションインターフェース
class IAnimation {
public:
    virtual ~IAnimation() = default;
    //モーションを取得する
    virtual const Motion& getMotion(unsigned index) const = 0;
    //モーション数を取得する
    virtual unsigned getMotionCount() const = 0;
    //モーション名を設定する
    virtual void setMotionName(const std::string& name, unsigned index) = 0;
    //ボーンを取得する
    virtual const Bone& getBone(unsigned index) const = 0;
    //ボーン数を取得する
    virtual unsigned getBoneCount() const = 0;
};
