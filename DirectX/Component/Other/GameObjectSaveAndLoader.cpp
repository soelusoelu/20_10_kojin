#include "GameObjectSaveAndLoader.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Utility/LevelLoader.h"

GameObjectSaveAndLoader::GameObjectSaveAndLoader(GameObject& gameObject)
    : Component(gameObject)
{
}

GameObjectSaveAndLoader::~GameObjectSaveAndLoader() = default;

void GameObjectSaveAndLoader::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getStringArray(inObj, "gameObjectNames", &mGameObjectNames);
    for (const auto& name : mGameObjectNames) {
        GameObjectCreater::create(name);
    }
}

void GameObjectSaveAndLoader::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setStringArray(alloc, inObj, "gameObjectNames", mGameObjectNames);
}

void GameObjectSaveAndLoader::addSaveGameObject(const std::string& name) {
    mGameObjectNames.emplace_back(name);
}
