#include "GameObject.h"
#include "GameObjectManager.h"
#include "../Transform/Transform3D.h"
#include "../Component/ComponentManager.h"

GameObject::GameObject() :
    mTransform(nullptr),
    mComponentManager(nullptr),
    mTag(""),
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

        computeWorldTransform();
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

void GameObject::setTag(const std::string& tag) {
    mTag = tag;
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

std::shared_ptr<GameObject> GameObject::create() {
    auto obj = std::make_shared<GameObject>();
    obj->initialize();
    return obj;
}

void GameObject::initialize() {
    if (mGameObjectManager) {
        mGameObjectManager->add(shared_from_this());
    }

    mTransform = std::make_unique<Transform3D>(shared_from_this());
    mComponentManager = std::make_unique<ComponentManager>();
}

void GameObject::computeWorldTransform() {
    if (mTransform->computeWorldTransform()) {
        mComponentManager->onUpdateWorldTransform();
    }
}
