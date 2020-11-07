#include "Transform3D.h"
#include "../GameObject/GameObject.h"
#include "../Utility/LevelLoader.h"

Transform3D::Transform3D() :
    mWorldTransform(Matrix4::identity),
    mPosition(Vector3::zero),
    mRotation(Quaternion::identity),
    mPivot(Vector3::zero),
    mScale(Vector3::one), 
    mParent(nullptr),
    mIsRecomputeTransform(true) {
}

Transform3D::~Transform3D() {
    if (mParent) {
        mParent.reset();
    }
    if (mChildren.empty()) {
        return;
    }
    for (const auto& child : mChildren) {
        //child->gameObject().destroy();
    }
}

bool Transform3D::computeWorldTransform() {
    if (mIsRecomputeTransform) {
        mWorldTransform = Matrix4::createTranslation(-mPivot); //中心 + ピボットを原点に
        mWorldTransform *= Matrix4::createScale(getScale());
        mWorldTransform *= Matrix4::createFromQuaternion(getRotation());
        mWorldTransform *= Matrix4::createTranslation(getPosition());

        mIsRecomputeTransform = false;

        return true;
    }
    return false;
}

const Matrix4& Transform3D::getWorldTransform() const {
    return mWorldTransform;
}

void Transform3D::setPosition(const Vector3& pos) {
    mPosition = pos;
    shouldRecomputeTransform();
}

Vector3 Transform3D::getPosition() const {
    auto root = mParent;
    auto pos = mPosition;
    while (root) {
        pos += root->mPosition;
        root = root->mParent;
    }
    return pos;
}

const Vector3& Transform3D::getLocalPosition() const {
    return mPosition;
}

void Transform3D::translate(const Vector3& translation) {
    mPosition += translation;
    shouldRecomputeTransform();
}

void Transform3D::translate(float x, float y, float z) {
    mPosition.x += x;
    mPosition.y += y;
    mPosition.z += z;
    shouldRecomputeTransform();
}

void Transform3D::setRotation(const Quaternion& rot) {
    mRotation = rot;
    shouldRecomputeTransform();
}

void Transform3D::setRotation(const Vector3& axis, float angle) {
    angle *= 0.5f;
    auto sinAngle = Math::sin(angle);

    mRotation.x = axis.x * sinAngle;
    mRotation.y = axis.y * sinAngle;
    mRotation.z = axis.z * sinAngle;
    mRotation.w = Math::cos(angle);

    shouldRecomputeTransform();
}

Quaternion Transform3D::getRotation() const {
    auto root = mParent;
    auto rotation = mRotation;
    while (root) {
        rotation = Quaternion::concatenate(rotation, root->mRotation);
        root = root->mParent;
    }
    return rotation;
}

const Quaternion& Transform3D::getLocalRotation() const {
    return mRotation;
}

void Transform3D::rotate(const Vector3& axis, float angle) {
    angle *= 0.5f;
    float sinAngle = Math::sin(angle);

    Quaternion inc;
    inc.x = axis.x * sinAngle;
    inc.y = axis.y * sinAngle;
    inc.z = axis.z * sinAngle;
    inc.w = Math::cos(angle);

    mRotation = Quaternion::concatenate(mRotation, inc);

    shouldRecomputeTransform();
}

void Transform3D::rotate(const Vector3& eulers) {
    rotate(Vector3::right, eulers.x);
    rotate(Vector3::up, eulers.y);
    rotate(Vector3::forward, eulers.z);
}

void Transform3D::setPivot(const Vector3& pivot) {
    mPivot = pivot;
    shouldRecomputeTransform();
}

const Vector3& Transform3D::getPivot() const {
    return mPivot;
}

void Transform3D::setScale(const Vector3& scale) {
    mScale = scale;
    shouldRecomputeTransform();
}

void Transform3D::setScale(float scale) {
    mScale.x = scale;
    mScale.y = scale;
    mScale.z = scale;
    shouldRecomputeTransform();
}

Vector3 Transform3D::getScale() const {
    auto root = mParent;
    auto scale = mScale;
    while (root) {
        scale.x *= root->mScale.x;
        scale.y *= root->mScale.y;
        scale.z *= root->mScale.z;
        root = root->mParent;
    }
    return scale;
}

const Vector3& Transform3D::getLocalScale() const {
    return mScale;
}

Vector3 Transform3D::forward() const {
    return Vector3::transform(Vector3::forward, mRotation);
}

Vector3 Transform3D::up() const {
    return Vector3::transform(Vector3::up, mRotation);
}

Vector3 Transform3D::right() const {
    return Vector3::transform(Vector3::right, mRotation);
}

void Transform3D::addChild(std::shared_ptr<Transform3D>& child) {
    mChildren.emplace_back(child);
    child->setParent(shared_from_this());
}

std::list<std::shared_ptr<Transform3D>> Transform3D::getChildren() const {
    return mChildren;
}

Transform3D& Transform3D::parent() const {
    return *mParent;
}

Transform3D& Transform3D::root() const {
    auto root = mParent;
    while (root) {
        auto p = root->mParent;
        if (!p) {
            break;
        }
        root = p;
    }
    return *root;
}

size_t Transform3D::getChildCount() const {
    return mChildren.size();
}

void Transform3D::loadProperties(const rapidjson::Value& inObj) {
    //位置、回転、スケールを読み込む
    JsonHelper::getVector3(inObj, "position", &mPosition);
    Vector3 rot;
    if (JsonHelper::getVector3(inObj, "rotation", &rot)) {
        rotate(rot);
    }
    JsonHelper::getVector3(inObj, "scale", &mScale);
    computeWorldTransform();
}

void Transform3D::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    //位置、回転、スケールを書き込む
    JsonHelper::setVector3(alloc, inObj, "position", mPosition);
    JsonHelper::setVector3(alloc, inObj, "rotation", mRotation.euler());
    JsonHelper::setVector3(alloc, inObj, "scale", mScale);
}

void Transform3D::setParent(const std::shared_ptr<Transform3D>& parent) {
    mParent = parent;
}

void Transform3D::shouldRecomputeTransform() {
    mIsRecomputeTransform = true;

    if (mChildren.empty()) {
        return;
    }
    for (const auto& child : mChildren) {
        child->mIsRecomputeTransform = true;
    }
}
