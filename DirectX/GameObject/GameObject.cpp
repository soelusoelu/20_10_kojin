#include "GameObject.h"
#include "GameObjectManager.h"
#include "../Transform/Transform3D.h"
#include "../Component/ComponentManager.h"

GameObject::GameObject() :
    mTransform(nullptr),
    mComponentManager(nullptr),
    mName(),
    mTag(),
    mIsActive(true) {
}

GameObject::~GameObject() {
    mComponentManager->finalize();
}

void GameObject::update() {
    mComponentManager->start();
    if (getActive()) {
        mComponentManager->update();
    }
    updateDestroyTimer();
}

void GameObject::lateUpdate() {
    if (getActive()) {
        mComponentManager->lateUpdate();

        mTransform->computeWorldTransform();
    }
}

void GameObject::loadProperties(const rapidjson::Value& inObj) {
    mTransform->loadProperties(inObj);
}

void GameObject::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    mTransform->saveProperties(alloc, inObj);
}

void GameObject::setActive(bool value) {
    mIsActive = value;

    mComponentManager->onEnable(value);
}

bool GameObject::getActive() const {
    return mIsActive;
}

const std::string& GameObject::name() const {
    return mName;
}

const std::string& GameObject::tag() const {
    return mTag;
}

Transform3D& GameObject::transform() const {
    return *mTransform;
}

ComponentManager& GameObject::componentManager() const {
    return *mComponentManager;
}

void GameObject::setGameObjectManager(GameObjectManager* manager) {
    mGameObjectManager = manager;
}

GameObjectManager& GameObject::getGameObjectManager() {
    return *mGameObjectManager;
}

std::shared_ptr<GameObject> GameObject::create(const std::string& name, const std::string& tag) {
    auto obj = std::make_shared<GameObject>();
    //名前とタグをそれぞれ設定
    obj->mName = name;
    obj->mTag = tag;
    //初期化
    obj->initialize();

    return obj;
}

void GameObject::initialize() {
    if (mGameObjectManager) {
        mGameObjectManager->add(shared_from_this());
    }

    mTransform = std::make_unique<Transform3D>();
    mComponentManager = std::make_unique<ComponentManager>();
}
