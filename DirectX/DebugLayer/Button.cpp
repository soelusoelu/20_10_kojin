#include "Button.h"
#include "../Sprite/Sprite.h"
#include "../Transform/Transform2D.h"

Button::Button(std::function<void()> onClick, const Vector2& pos, const Vector2& dims) :
    mOnClick(onClick),
    mPosition(pos),
    mDimensions(dims),
    mHighlighted(false) {
}

Button::~Button() = default;

void Button::setClickFunc(std::function<void()> onClick) {
    mOnClick = onClick;
}

const Vector2& Button::getPosition() const {
    return mPosition;
}

void Button::setHighlighted(bool set) {
    mHighlighted = set;
}

bool Button::getHighlighted() const {
    return mHighlighted;
}

bool Button::containsPoint(const Vector2& pt) const {
    bool no = pt.x < (mPosition.x) ||
        pt.x >(mPosition.x + mDimensions.x) ||
        pt.y < (mPosition.y) ||
        pt.y >(mPosition.y + mDimensions.y);
    return !no;
}

void Button::onClick() {
    if (mOnClick) {
        mOnClick();
    }
}



SpriteButton::SpriteButton(std::function<void()> onClick, const std::string& fileName, const Vector2& pos) :
    mOnClick(onClick),
    mSprite(std::make_unique<Sprite>(fileName)),
    mHighlighted(false) {
    mSprite->transform().setPosition(pos);
}

SpriteButton::~SpriteButton() = default;

void SpriteButton::setClickFunc(std::function<void()> onClick) {
    mOnClick = onClick;
}

const Vector2& SpriteButton::getPosition() const {
    return mSprite->transform().getPosition();
}

void SpriteButton::setHighlighted(bool set) {
    mHighlighted = set;
}

bool SpriteButton::getHighlighted() const {
    return mHighlighted;
}

bool SpriteButton::containsPoint(const Vector2& pt) const {
    const auto& pos = mSprite->transform().getPosition();
    const auto& size = mSprite->getTextureSize();

    bool no = pt.x < (pos.x) ||
        pt.x >(pos.x + size.x) ||
        pt.y < (pos.y) ||
        pt.y >(pos.y + size.y);
    return !no;
}

void SpriteButton::onClick() {
    if (mOnClick) {
        mOnClick();
    }
}

void SpriteButton::draw(const Matrix4& proj) const {
    mSprite->transform().computeWorldTransform();
    mSprite->draw(proj);
}
