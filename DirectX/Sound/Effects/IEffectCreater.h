#pragma once

#include "ISoundEffect.h"
#include <xaudio2.h>

//エフェクト作成インターフェース
class IEffectCreater {
public:
    virtual ~IEffectCreater() = default;
    virtual int createEffect(ISoundEffect* target, bool isApply = true) = 0;
    virtual int createEffect(IUnknown* target, bool isApply = true) = 0;
};
