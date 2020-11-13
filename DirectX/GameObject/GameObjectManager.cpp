#include "GameObjectManager.h"
#include "GameObject.h"
#include "../DebugLayer/DebugUtility.h"
#include "../DebugLayer/Hierarchy.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"
#include <algorithm>
#include <iterator>

GameObjectManager::GameObjectManager() :
    mUpdatingGameObjects(false) {
    GameObject::setGameObjectManager(this);
}

GameObjectManager::~GameObjectManager() {
    GameObject::setGameObjectManager(nullptr);
}

void GameObjectManager::update() {
    mUpdatingGameObjects = true;
    for (const auto& gameObject : mGameObjects) {
        gameObject->update();
    }
    for (const auto& gameObject : mGameObjects) {
        gameObject->lateUpdate();
    }
    mUpdatingGameObjects = false;

    movePendingToMain();

    remove();

    DebugUtility::hierarchy().setGameObjectToButton(mGameObjects);
}

void GameObjectManager::add(const GameObjectPtr & add) {
    if (mUpdatingGameObjects) {
        mPendingGameObjects.emplace_back(add);
    } else {
        mGameObjects.emplace_back(add);
    }
}

void GameObjectManager::clear() {
    StringSet set{};
    clearExceptSpecified(set);
}

void GameObjectManager::clearExceptSpecified(const StringSet & tags) {
    auto excepts = tags;
    excepts.emplace("Camera");
    excepts.emplace("DirectionalLight");

    auto itr = mGameObjects.begin();
    while (itr != mGameObjects.end()) {
        if (excepts.find((*itr)->tag()) == excepts.end()) {
            itr = mGameObjects.erase(itr);
        } else {
            ++itr;
        }
    }
    mPendingGameObjects.clear();
}

const std::shared_ptr<GameObject>& GameObjectManager::find(const std::string & tag) const {
    for (const auto& gameObject : mGameObjects) {
        if (!gameObject->getActive()) {
            continue;
        }
        if (gameObject->tag() == tag) {
            return gameObject;
        }
    }
    for (const auto& gameObject : mPendingGameObjects) {
        if (!gameObject->getActive()) {
            continue;
        }
        if (gameObject->tag() == tag) {
            return gameObject;
        }
    }
    //最後まで見つからなければnullptrを返す
    return nullptr;
}

std::vector<std::shared_ptr<GameObject>> GameObjectManager::findGameObjects(const std::string& tag) const {
    GameObjectPtrArray gameObjectArray;
    for (const auto& gameObject : mGameObjects) {
        if (!gameObject->getActive()) {
            continue;
        }
        if (gameObject->tag() == tag) {
            gameObjectArray.emplace_back(gameObject);
        }
    }
    for (const auto& gameObject : mPendingGameObjects) {
        if (!gameObject->getActive()) {
            continue;
        }
        if (gameObject->tag() == tag) {
            gameObjectArray.emplace_back(gameObject);
        }
    }

    return gameObjectArray;
}

void GameObjectManager::setNameNumber(std::string& name) const {
    bool isEnd = false;
    checkNameNumber(name, isEnd);
}

void GameObjectManager::remove() {
    auto itr = mGameObjects.begin();
    while (itr != mGameObjects.end()) {
        if ((*itr)->isDead()) {
            itr = mGameObjects.erase(itr);
        } else {
            ++itr;
        }
    }
}

void GameObjectManager::movePendingToMain() {
    if (mPendingGameObjects.empty()) {
        return;
    }
    std::copy(mPendingGameObjects.begin(), mPendingGameObjects.end(), std::back_inserter(mGameObjects));
    mPendingGameObjects.clear();
}

void GameObjectManager::checkNameNumber(std::string& name, bool& isEnd, int number) const {
    std::string numberedName = name;
    //再帰してたら名前の後ろに数字をつける
    if (number > 0) {
        numberedName += StringUtil::intToString(number);
    }
    for (const auto& obj : mGameObjects) {
        if (obj->name() == numberedName) {
            //名前が一致するゲームオブジェクトが存在したら、末尾の数字を1増やして再帰する
            ++number;
            checkNameNumber(name, isEnd, number);
        }
    }

    if (isEnd) {
        return;
    }
    name = numberedName;
    isEnd = true;
}
