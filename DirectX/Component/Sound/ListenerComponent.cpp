#include "ListenerComponent.h"
#include "../../Sound/3D/Listener/Sound3DListener.h"
#include "../../Sound/XAudio2/SoundEngine.h"
#include "../../Transform/Transform3D.h"

ListenerComponent::ListenerComponent(GameObject& gameObject) :
    Component(gameObject),
    mListener(std::make_shared<Sound3DListener>()) {
}

ListenerComponent::~ListenerComponent() = default;

void ListenerComponent::awake() {
    //サウンドエンジンにリスナーを登録
    SoundEngine::instance().setListener(mListener);
}

void ListenerComponent::update() {
    mListener->setFrontAndTop(transform().forward(), transform().up());
    mListener->setPosition(transform().getPosition());
}

void ListenerComponent::setCone(Sound3DCone* cone) {
    mListener->setCone(cone);
}

const Sound3DListenerParam& ListenerComponent::getListener() const {
    return mListener->getListener();
}
