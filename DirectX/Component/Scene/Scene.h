#pragma once

#include "../Component.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

using ValuePassMap = std::unordered_map<std::string, std::any>;

class Scene : public Component, public std::enable_shared_from_this<Scene> {
public:
    Scene(GameObject& gameObject);
    virtual ~Scene();

    //前のシーンから値を受け取る
    virtual void getValueFromPreviousScene(const ValuePassMap& values) {};

    //設定した次のシーンに遷移
    void next(const std::string& next);
    //次のシーンの取得
    const std::string& getNext() const;
    //次のシーンに渡す値を設定する
    void addValuePassToNextScene(const std::string& valueName, const std::any& value);
    //次のシーンに渡す値を取得する
    const ValuePassMap& getValuePassToNextScene() const;

private:
    std::string mNext;
    ValuePassMap mValuesPassToNextScene;
};
