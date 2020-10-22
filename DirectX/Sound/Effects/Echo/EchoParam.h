#pragma once

#include <xapofx.h>

//エコーパラメータ
struct EchoParam {
    //エフェクトの強さ(0 ～ 1)
    float wetDryMix;
    //どのくらい残響を残すか(0 ～ 1)
    float feedback;
    //どのくらい遅らせて重ねるか(単位: ms)
    float delay;

    EchoParam() :
        wetDryMix(FXECHO_DEFAULT_WETDRYMIX),
        feedback(FXECHO_DEFAULT_FEEDBACK),
        delay(FXECHO_DEFAULT_DELAY) {
    }
};
