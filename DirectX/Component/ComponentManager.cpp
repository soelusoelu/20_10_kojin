﻿#include "ComponentManager.h"
#include "Component.h"
#include "../Utility/LevelLoader.h"

ComponentManager::ComponentManager() = default;
ComponentManager::~ComponentManager() = default;

void ComponentManager::start() {
    for (const auto& comp : mComponents) {
        comp->start();
    }
}

void ComponentManager::update() const  {
    for (const auto& comp : mComponents) {
        comp->update();
    }
}

void ComponentManager::lateUpdate() const {
    for (const auto& comp : mComponents) {
        comp->lateUpdate();
    }
}

void ComponentManager::finalize() {
    for (const auto& comp : mComponents) {
        comp->finalize();
    }

    mComponents.clear();
}

void ComponentManager::addComponent(const ComponentPtr& component) {
    mComponents.emplace_back(component);
}

void ComponentManager::onEnable(bool value) const {
    for (const auto& comp : mComponents) {
        comp->onEnable(value);
    }
}

const std::list<std::shared_ptr<Component>>& ComponentManager::getAllComponents() const {
    return mComponents;
}

void ComponentManager::saveComponents(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    for (const auto& c : mComponents) {
        saveComponent(alloc, inObj, *c);
    }
}

void ComponentManager::saveComponent(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* outArray, const Component& component) const {
    //Jsonオブジェクトを作成する
    rapidjson::Value obj(rapidjson::kObjectType);
    //コンポーネント名を保存
    JsonHelper::setString(alloc, &obj, "type", component.getComponentName());

    //プロパティ用オブジェクトを作成
    rapidjson::Value props(rapidjson::kObjectType);
    //コンポーネントのプロパティを保存する
    component.saveProperties(alloc, &props);
    //Jsonオブジェクトに追加
    obj.AddMember("properties", props, alloc);

    //コンポーネント配列に追加
    outArray->PushBack(obj, alloc);
}
