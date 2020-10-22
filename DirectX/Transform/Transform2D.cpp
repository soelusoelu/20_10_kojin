#include "Transform2D.h"
#include "../System/Window.h"

Transform2D::Transform2D() :
    mWorldTransform(Matrix4::identity),
    mPosition(Vector2::zero),
    mRotation(0.f),
    mPivot(Vector2::zero),
    mScale(Vector2::one),
    mSize(Vector2::zero),
    mIsRecomputeTransform(true) {
}

Transform2D::~Transform2D() = default;

bool Transform2D::computeWorldTransform() {
    if (mIsRecomputeTransform) {
        mWorldTransform = Matrix4::createScale(Vector3(mSize, 1.f)); //テクスチャサイズに
        mWorldTransform *= Matrix4::createTranslation(Vector3(-mPivot, 0.f)); //中心 + ピボットを原点に

        auto w = static_cast<float>(Window::width()) / static_cast<float>(Window::standardWidth());
        auto h = static_cast<float>(Window::height()) / static_cast<float>(Window::standardHeight());
        mWorldTransform *= Matrix4::createScale(Vector3(getScale() * Vector2(w, h), 1.f));
        mWorldTransform *= Matrix4::createRotationZ(mRotation);
        mWorldTransform *= Matrix4::createTranslation(Vector3(getPosition() * Vector2(w, h), 1.f));

        mIsRecomputeTransform = false;

        return true;
    }
    return false;
}

const Matrix4& Transform2D::getWorldTransform() const {
    return mWorldTransform;
}

void Transform2D::setPosition(const Vector2 & pos) {
    mPosition.x = pos.x;
    mPosition.y = pos.y;
    shouldRecomputeTransform();
}

const Vector2& Transform2D::getPosition() const {
    return mPosition;
}

void Transform2D::translate(const Vector2 & translation) {
    mPosition.x += translation.x;
    mPosition.y += translation.y;
    shouldRecomputeTransform();
}

void Transform2D::translate(float x, float y) {
    mPosition.x += x;
    mPosition.y += y;
    shouldRecomputeTransform();
}

void Transform2D::setRotation(float angle) {
    mRotation = angle;
    shouldRecomputeTransform();
}

float Transform2D::getRotation() const {
    return mRotation;
}

void Transform2D::rotate(float angle) {
    mRotation += angle;
    if (mRotation > 180.f) {
        mRotation += -360.f;
    } else if (mRotation < -180.f) {
        mRotation += 360.f;
    }
    shouldRecomputeTransform();
}

void Transform2D::setPivot(Pivot pivot) {
    switch (pivot) {
    case Pivot::LEFT_TOP:
        mPivot = Vector2::zero;
        break;
    case Pivot::CENTER_TOP:
        mPivot.x = mSize.x / 2.f;
        break;
    case Pivot::RIGHT_TOP:
        mPivot.x = mSize.x;
        break;
    case Pivot::CENTER_LEFT:
        mPivot.y = mSize.y / 2.f;
        break;
    case Pivot::CENTER:
        mPivot = mSize / 2.f;
        break;
    case Pivot::CENTER_RIGHT:
        mPivot = Vector2(mSize.x, mSize.y / 2.f);
        break;
    case Pivot::LEFT_BOTTOM:
        mPivot.y = mSize.y;
        break;
    case Pivot::CETNER_BOTTOM:
        mPivot = Vector2(mSize.x / 2.f, mSize.y);
        break;
    case Pivot::RIGHT_BOTTOM:
        mPivot = mSize;
        break;
    default:
        break;
    }
    shouldRecomputeTransform();
}

const Vector2& Transform2D::getPivot() const {
    return mPivot;
}

void Transform2D::setScale(const Vector2 & scale) {
    mScale = scale;
    shouldRecomputeTransform();
}

void Transform2D::setScale(float scale) {
    mScale.x = scale;
    mScale.y = scale;
    shouldRecomputeTransform();
}

const Vector2& Transform2D::getScale() const {
    return mScale;
}

void Transform2D::setSize(const Vector2 & size) {
    mSize = size;
    shouldRecomputeTransform();
}

const Vector2& Transform2D::getSize() const {
    return mSize;
}

void Transform2D::shouldRecomputeTransform() {
    mIsRecomputeTransform = true;
}
