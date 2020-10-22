#include "SpriteManager.h"
#include "../Component/Sprite/Sprite3D.h"
#include "../Component/Sprite/SpriteComponent.h"

SpriteManager::SpriteManager() {
    SpriteComponent::setSpriteManager(this);
    Sprite3D::setSpriteManager(this);
}

SpriteManager::~SpriteManager() {
    SpriteComponent::setSpriteManager(nullptr);
    Sprite3D::setSpriteManager(nullptr);
}

void SpriteManager::update() {
    remove();
}

void SpriteManager::drawComponents(const Matrix4& proj) const {
    if (mSpriteComponents.empty()) {
        return;
    }

    for (const auto& sprite : mSpriteComponents) {
        if (!sprite->getActive() || sprite->isDead()) {
            continue;
        }
        sprite->draw(proj);
    }
}

void SpriteManager::draw3Ds(const Matrix4& view, const Matrix4& proj) const {
    if (mSprite3Ds.empty()) {
        return;
    }

    auto cancelRotation = view;
    cancelRotation.m[3][0] = 0.f;
    cancelRotation.m[3][1] = 0.f;
    cancelRotation.m[3][2] = 0.f;
    cancelRotation.inverse();

    for (const auto& sprite : mSprite3Ds) {
        if (!sprite->getActive() || sprite->isDead()) {
            continue;
        }
        if (sprite->isBillboard()) {
            sprite->drawBillboard(cancelRotation, view * proj);
        } else {
            sprite->draw(view * proj);
        }
    }
}

void SpriteManager::addComponent(const SpriteComponentPtr& add) {
    int order = add->getDrawOrder();
    auto itr = mSpriteComponents.begin();
    for (; itr != mSpriteComponents.end(); ++itr) {
        if (order < (*itr)->getDrawOrder()) {
            break;
        }
    }
    mSpriteComponents.insert(itr, add);
}

void SpriteManager::add3D(const Sprite3DPtr& add) {
    mSprite3Ds.emplace_back(add);
}

void SpriteManager::clear() {
    mSpriteComponents.clear();
    mSprite3Ds.clear();
}

void SpriteManager::remove() {
    auto itr = mSpriteComponents.begin();
    while (itr != mSpriteComponents.end()) {
        if ((*itr)->isDead()) {
            itr = mSpriteComponents.erase(itr);
        } else {
            ++itr;
        }
    }

    auto itr2 = mSprite3Ds.begin();
    while (itr2 != mSprite3Ds.end()) {
        if ((*itr2)->isDead()) {
            itr2 = mSprite3Ds.erase(itr2);
        } else {
            ++itr2;
        }
    }
}
