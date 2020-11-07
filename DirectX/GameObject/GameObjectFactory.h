#pragma once

#include <rapidjson/document.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class GameObject;

class GameObjectFactory {
    using ComponentFunc = std::function<void(GameObject&, const std::string&, const rapidjson::Value&)>;

public:
    GameObjectFactory();
    ~GameObjectFactory();
    //ファイルからゲームオブジェクト生成
    std::shared_ptr<GameObject> createGameObjectFromFile(const std::string& type, const std::string& directoryPath = "Assets\\Data\\");

private:
    GameObjectFactory(const GameObjectFactory&) = delete;
    GameObjectFactory& operator=(const GameObjectFactory&) = delete;

    //ゲームオブジェクトを生成する
    std::shared_ptr<GameObject> createGameObject(const rapidjson::Document& inDocument, const std::string& type);
    //ゲームオブジェクトのタグを取得する
    std::string loadTag(const rapidjson::Document& inDocument);
    //ゲームオブジェクトプロパティの読み込み
    void loadGameObjectProperties(GameObject& gameObject, const rapidjson::Document& inDocument);
    //コンポーネントの読み込み
    void loadComponents(GameObject& gameObject, const rapidjson::Document& inDocument) const;
    //各コンポーネントの読み込み
    void loadComponent(GameObject& gameObject, const rapidjson::Value& component) const;

    //有効な型か
    bool isValidType(std::string& outType, const rapidjson::Value& inObj) const;

private:
    std::unordered_map<std::string, ComponentFunc> mComponents;

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
