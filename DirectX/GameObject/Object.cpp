#include "Object.h"
#include "../Device/Time.h"

Object::Object() :
    mDestroyTimer(nullptr),
    mIsDead(false) {
}

Object::~Object() = default;

void Object::destroy() {
    mIsDead = true;
}

void Object::destroy(float sec) {
    if (mDestroyTimer) {
        return;
    }
    mDestroyTimer = std::make_unique<Time>(sec);
}

bool Object::isDead() const {
    return mIsDead;
}

void Object::updateDestroyTimer() {
    if (!mDestroyTimer) {
        return;
    }
    mDestroyTimer->update();
    if (mDestroyTimer->isTime()) {
        destroy();
    }
}
