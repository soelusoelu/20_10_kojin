#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <string>

class DrawString;
class FixedDebugInformation;
class Hierarchy;
class Inspector;
class Log;
class Pause;
class LineRenderer2D;
class LineRenderer3D;

class DebugUtility {
    DebugUtility() = delete;
    ~DebugUtility() = delete;
    DebugUtility(const DebugUtility&) = delete;
    DebugUtility& operator=(const DebugUtility&) = delete;

public:
    static void create();
    static void loadProperties(const rapidjson::Value& inObj);
    static void initialize();
    static void finalize();
    static void update();
    //messageを新規ウィンドウに表示
    //重大なバグを表示する用
    static void windowMessage(const std::string& message);
    static void draw(const Matrix4& proj);
    //DrawStringが保持する文字列をすべて削除
    static void drawStringClear();
    static Log& log();
    static FixedDebugInformation* fixedDebugInfo();
    static Hierarchy& hierarchy();
    static Inspector& inspector();
    static Pause& pause();
    static LineRenderer2D& lineRenderer2D();
    static LineRenderer3D& lineRenderer3D();

private:
    static inline DrawString* mDrawString = nullptr;
    static inline Log* mLog = nullptr;
    static inline FixedDebugInformation* mFixedDebugInfo = nullptr;
    static inline Hierarchy* mHierarchy = nullptr;
    static inline Inspector* mInspector = nullptr;
    static inline Pause* mPause = nullptr;
    static inline LineRenderer2D* mLineRenderer2D = nullptr;
    static inline LineRenderer3D* mLineRenderer3D = nullptr;
};
