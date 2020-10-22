#pragma once

#include "../Component.h"
#include <string>
#include <unordered_set>

class Scene : public Component {
    using StringSet = std::unordered_set<std::string>;

public:
    Scene(GameObject& gameObject);
    ~Scene();
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    //設定した次のシーンに遷移
    void next(const std::string& next);
    //次のシーンの取得
    const std::string& getNext() const;
    //次のシーンまで持ち越すオブジェクトの登録
    void addObjectToNext(const std::string& tag);
    //次のシーンまで持ち越すオブジェクトの取得
    StringSet getObjectToNext() const;

private:
    std::string mNext;
    StringSet mTagsToNext;
};
