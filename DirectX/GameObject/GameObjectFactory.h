#pragma once

#include <rapidjson/document.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class GameObject;

class GameObjectFactory {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using ComponentFunc = std::function<void(GameObject&, const std::string&, const rapidjson::Value&)>;

public:
    GameObjectFactory();
    ~GameObjectFactory();
    //ファイルからゲームオブジェクト生成
    GameObjectPtr loadGameObjectFromFile(const std::string& type) const;

private:
    GameObjectFactory(const GameObjectFactory&) = delete;
    GameObjectFactory& operator=(const GameObjectFactory&) = delete;

    //アクターの読み込み
    GameObjectPtr loadGameObjectProperties(const rapidjson::Value& inArray, const std::string& type) const;
    //コンポーネントの読み込み
    void loadComponents(GameObject& gameObject, const rapidjson::Value& inArray) const;

private:
    std::unordered_map<std::string, ComponentFunc> mComponents;
    rapidjson::Document mDocument;

    static inline bool mInstantiated = false;
};

class GameObjectCreater {
public:
    static void initialize();
    static void finalize();
    static std::shared_ptr<GameObject> create(const std::string& type);

private:
    GameObjectCreater() = delete;
    ~GameObjectCreater() = delete;
    GameObjectCreater(const GameObjectCreater&) = delete;
    GameObjectCreater& operator=(const GameObjectCreater&) = delete;

private:
    static inline GameObjectFactory* mFactory = nullptr;
};
