#include "Component.h"
#include "../GameObject/GameObject.h"

Component::Component(GameObject& gameObject) :
    mGameObject(gameObject),
    mComponentName("") {
}

Component::~Component() = default;

GameObject& Component::gameObject() const {
    return mGameObject;
}

Transform3D& Component::transform() const {
    return mGameObject.transform();
}

const std::string& Component::getComponentName() const {
    return mComponentName;
}

ComponentManager& Component::componentManager() const {
    return mGameObject.componentManager();
}
