#include "Sound3DListener.h"
#include "../../../Device/Time.h"

Sound3DListener::Sound3DListener() :
    mListener(),
    mPreviousPos(Vector3::zero) {
    mListener.cone = &LISTENER_DIRECTIONAL_CONE;
}

Sound3DListener::~Sound3DListener() = default;

void Sound3DListener::update() {
    mListener.velocity = (mListener.position - mPreviousPos) / Time::deltaTime;
    mPreviousPos = mListener.position;
}

void Sound3DListener::setPosition(const Vector3& pos) {
    mListener.position = pos;
}

void Sound3DListener::setFrontAndTop(const Vector3& front, const Vector3& top) {
    mListener.orientFront = front;
    mListener.orientTop = top;
}

void Sound3DListener::setCone(Sound3DCone* cone) {
    mListener.cone = cone;
}

const Sound3DListenerParam& Sound3DListener::getListener() const {
    return mListener;
}
