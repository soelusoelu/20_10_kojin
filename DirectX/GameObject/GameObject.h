#pragma once

#include "Object.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class ComponentManager;
class GameObjectManager;
class Transform3D;

class GameObject final : public Object, public std::enable_shared_from_this<GameObject> {
public:
    GameObject();
    ~GameObject();

    //更新
    void update();
    //遅延更新
    void lateUpdate();

    //ロード/セーブ
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const;

    //アクティブ指定
    void setActive(bool value);
    //アクティブ状態の取得
    bool getActive() const;

    //名前の取得
    const std::string& name() const;
    //タグの取得
    const std::string& tag() const;

    //トランスフォームの取得
    Transform3D& transform() const;
    //コンポーネント管理者の取得
    ComponentManager& componentManager() const;

    //GameObjectManagerの登録
    static void setGameObjectManager(GameObjectManager* manager);
    //GameObjectManagerの取得
    GameObjectManager& getGameObjectManager();

    //ゲームオブジェクトを生成
    static std::shared_ptr<GameObject> create(const std::string& name, const std::string& tag);

private:
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    //初期化
    void initialize();
    //ワールド行列の更新
    void computeWorldTransform();

private:
    std::unique_ptr<Transform3D> mTransform;
    std::unique_ptr<ComponentManager> mComponentManager;
    std::string mName;
    std::string mTag;
    bool mIsActive;

    static inline GameObjectManager* mGameObjectManager = nullptr;
};
