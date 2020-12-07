#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <string>
#include <utility>

class DrawString;

class Log {
public:
    Log();
    ~Log();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void log(const std::string& message);
    void logError(const std::string& message);
    void logWarning(const std::string& message);
    void clear();
    void drawLogs(DrawString* drawString) const;

private:
    void addLog(const std::string& message, const Vector3& color);
    void adjustCapacity();

private:
    //表示する文字列と色
    std::list<std::pair<std::string, Vector3>> mLogs;
    //文字のスケール
    Vector2 mScale;
    //画面に表示するログの行数
    int mNumRowsToDisplay;
};
