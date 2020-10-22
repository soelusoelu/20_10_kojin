#include "ComponentManager.h"
#include "Component.h"

ComponentManager::ComponentManager() = default;
ComponentManager::~ComponentManager() = default;

void ComponentManager::start() {
    if (mStartComponents.empty()) {
        return;
    }

    for (const auto& comp : mStartComponents) {
        comp->start();

        mComponents.emplace_back(comp);
    }
    mStartComponents.clear();
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
    for (const auto& comp : mStartComponents) {
        comp->finalize();
    }
    for (const auto& comp : mComponents) {
        comp->finalize();
    }

    mStartComponents.clear();
    mComponents.clear();
}

void ComponentManager::addComponent(const ComponentPtr& component) {
    mStartComponents.emplace_back(component);
}

void ComponentManager::onUpdateWorldTransform() const {
    for (const auto& comp : mStartComponents) {
        comp->onUpdateWorldTransform();
    }
    for (const auto& comp : mComponents) {
        comp->onUpdateWorldTransform();
    }
}

void ComponentManager::onEnable(bool value) const {
    for (const auto& comp : mStartComponents) {
        comp->onEnable(value);
    }
    for (const auto& comp : mComponents) {
        comp->onEnable(value);
    }
}

const std::list<std::shared_ptr<Component>>& ComponentManager::getAllComponents() const {
    return mComponents;
}
