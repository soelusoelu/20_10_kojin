#pragma once

#include "../Math/Math.h"
#include <string>

class Debug {
    Debug() = delete;
    ~Debug() = delete;
    Debug(const Debug&) = delete;
    Debug& operator=(const Debug&) = delete;

public:
    static void windowMessage(const std::string& message);
    static void log(const std::string& message);
    static void logError(const std::string& message);
    static void logWarning(const std::string& message);
    static void logClear();
    static void renderPoint(const Vector3& point, const Vector3& color = ColorPalette::white);
    static void renderLine(const Vector2& p1, const Vector2& p2, const Vector3& color = ColorPalette::white);
    static void renderLine(const Vector3& p1, const Vector3& p2, const Vector3& color = ColorPalette::white);
};
