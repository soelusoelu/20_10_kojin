#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class SpriteButton;

class Pause {
public:
    Pause();
    ~Pause();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void update();
    bool isPausing() const;
    void drawButton(const Matrix4& proj) const;

private:
    std::unique_ptr<SpriteButton> mButton;
    std::string mFileName;
    Vector2 mOffset;
    bool mIsPausing;
};
