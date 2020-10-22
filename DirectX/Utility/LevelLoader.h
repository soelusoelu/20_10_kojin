#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Game;
class GameObject;

class LevelLoader {
public:
    //jsonファイルの読み込み
    static bool loadJSON(const std::string& filePath, rapidjson::Document* outDoc);
    //グローバルデータを読み込む
    static void loadGlobal(Game* root, const std::string& filePath);
    //保存
    static void saveLevel(const std::string& filePath);
    //UI情報の保存
    static void saveUI(std::list<std::shared_ptr<GameObject>> uiList, const std::string& filePath);

private:
    LevelLoader() = delete;
    ~LevelLoader() = delete;
    LevelLoader(const LevelLoader&) = delete;
    LevelLoader& operator=(const LevelLoader&) = delete;

    //アクターの保存
    //void saveActors(rapidjson::Document::AllocatorType& alloc, ActorManager* manager, rapidjson::Value* inArray) const;
    //コンポーネントの保存
    //void saveComponents(rapidjson::Document::AllocatorType& alloc, const std::shared_ptr<Actor> actor, rapidjson::Value* inArray) const;
};

class JsonHelper {
public:
    //解析に成功したらtrue, 失敗したらfalse
    //true: プロパティの名前から値をセット
    //false: 値を変えずにreturn
    static bool getInt(const rapidjson::Value& inObject, const char* inProperty, int* out);
    static bool getFloat(const rapidjson::Value& inObject, const char* inProperty, float* out);
    static bool getString(const rapidjson::Value& inObject, const char* inProperty, std::string* out);
    static bool getBool(const rapidjson::Value& inObject, const char* inProperty, bool* out);
    static bool getVector2(const rapidjson::Value& inObject, const char* inProperty, Vector2* out);
    static bool getVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3* out);
    static bool getVector4(const rapidjson::Value& inObject, const char* inProperty, Vector4* out);
    static bool getQuaternion(const rapidjson::Value& inObject, const char* inProperty, Quaternion* out);
    static bool getStringArray(const rapidjson::Value& inObject, const char* inProperty, std::vector<std::string>* out);

    static void setInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, int value);
    static void setFloat(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, float value);
    static void setString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, const std::string& value);
    static void setBool(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, bool value);
    static void setVector2(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, const Vector2& value);
    static void setVector3(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, const Vector3& value);
    static void setVector4(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, const Vector4& value);
    static void setQuaternion(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, const Quaternion& value);
};
