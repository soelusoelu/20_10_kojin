#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>

class DrawString;

class FixedDebugInformation {
public:
    FixedDebugInformation(DrawString* drawString);
    ~FixedDebugInformation();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void draw() const;
    //フレームレート表示
    void drawFPS(float fps);

private:
    DrawString* mDrawString;
    //文字のスケール
    Vector2 mScale;
    //FPSを表示する位置 固定
    Vector2 mFPSPos;
    //フレームレート
    float mFPS;
};
