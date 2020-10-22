#pragma once

//簡易版リバーブパラメータ
struct SimpleReverbParam {
    //壁反射の硬さ(0.f ～ 1.f)
    float diffusion;
    //部屋のサイズ(0.f ～ 1.f)
    float roomSize;

    SimpleReverbParam() :
        diffusion(0.9f),
        roomSize(0.6f) {
    }
};
