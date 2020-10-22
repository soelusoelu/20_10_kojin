#pragma once

class Easing {
public:
    static float easeInSin(float value);
    static float easeOutSin(float value);
    static float easeInOutSin(float value);

    static float easeInCubic(float value);
    static float easeOutCubic(float value);
    static float easeInOutCubic(float value);

    static float easeInBack(float value);
    static float easeOutBack(float value);
    static float easeInOutBack(float value);
};
