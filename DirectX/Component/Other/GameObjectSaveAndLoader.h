#pragma once

#include "../Component.h"
#include <string>
#include <vector>

//ゲームオブジェクトのセーブ/ロードを請け負うクラス
class GameObjectSaveAndLoader : public Component {
    using StringArray = std::vector<std::string>;

public:
    GameObjectSaveAndLoader(GameObject& gameObject);
    ~GameObjectSaveAndLoader();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;

    //保存するゲームオブジェクトを追加する
    void addSaveGameObject(const std::string& name);

private:
    GameObjectSaveAndLoader(const GameObjectSaveAndLoader&) = delete;
    GameObjectSaveAndLoader& operator=(const GameObjectSaveAndLoader&) = delete;

private:
    //保存するゲームオブジェクト名配列
    StringArray mGameObjectNames;
};
