#pragma once

#include "ComponentManager.h"
#include "../GameObject/Object.h"
#include <rapidjson/document.h>
#include <any>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class GameObject;
class Transform3D;
class ComponentManager;

class Component : public Object {
public:
    Component() = delete;
    Component(GameObject& gameObject);
    virtual ~Component();
    //loadPropertiesの直後に呼び出される
    virtual void awake() {};
    //全コンポーネントがロードされたあとに呼ばれる
    virtual void start() {};
    //毎フレーム呼ばれる
    virtual void update() {};
    //全ゲームオブジェクトのアップデート後に呼ばれる
    virtual void lateUpdate() {};
    //終了処理
    virtual void finalize() {};
    //アクティブ・非アクティブ時の切り替え
    virtual void onEnable(bool value) {};
    //ロード/セーブ
    virtual void loadProperties(const rapidjson::Value& inObj) {};
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {};
    //Inspectorに表示する情報
    virtual void drawInspector() {};

    //コンポーネントがアタッチされているゲームオブジェクトを返す
    GameObject& gameObject() const;
    //トランスフォームの取得
    Transform3D& transform() const;
    //コンポーネントの名前を返す
    const std::string& getComponentName() const;

    //コンポーネントの取得
    template<typename T>
    std::shared_ptr<T> getComponent() const {
        return componentManager().getComponent<T>();
    }

    //指定した型のコンポーネントをすべて取得
    template<typename T>
    std::vector<std::shared_ptr<T>> getComponents() const {
        return componentManager().getComponents<T>();
    }

    //コンポーネントの追加
    template <typename T>
    static std::shared_ptr<T> addComponent(GameObject& gameObject, const std::string& componentName) {
        auto t = std::make_shared<T>(gameObject);
        t->mComponentName = componentName;
        t->componentManager().addComponent(t);
        t->awake();
        t->start();
        return t;
    }

    //自身にコンポーネントを追加する
    template <typename T>
    std::shared_ptr<T> addComponent(const std::string& componentName) {
        return addComponent<T>(mGameObject, componentName);
    }

    //指定されたプロパティでコンポーネントを生成
    template <typename T>
    static void create(GameObject& gameObject, const std::string& componentName, const rapidjson::Value& inObj) {
        auto t = std::make_shared<T>(gameObject);
        t->mComponentName = componentName;
        t->componentManager().addComponent(t);
        t->loadProperties(inObj);
        t->awake();
    }

private:
    //自身を管理しているマネージャーを返す
    ComponentManager& componentManager() const;

private:
    GameObject& mGameObject;
    std::string mComponentName;
};
