#include "Camera.h"
#include "../../System/Window.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

Camera::Camera(GameObject& gameObject) :
    Component(gameObject),
    mLookAt(Vector3::forward * 10.f),
    mFOV(45.f),
    mNearClip(0.1f),
    mFarClip(100.f),
    mView(Matrix4::identity),
    mProjection(Matrix4::identity) {
}

Camera::~Camera() = default;

void Camera::awake() {
    calcLookAt();
    calcPerspectiveFOV(Window::standardWidth(), Window::standardHeight());
}

void Camera::lateUpdate() {
    calcLookAt();
}

void Camera::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getFloat(inObj, "fov", &mFOV);
    JsonHelper::getFloat(inObj, "nearClip", &mNearClip);
    JsonHelper::getFloat(inObj, "farClip", &mFarClip);
}

void Camera::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FOV", mFOV);
    inspect->emplace_back("NearClip", mNearClip);
    inspect->emplace_back("FarClip", mFarClip);
}

const Matrix4& Camera::getView() const {
    return mView;
}

const Matrix4& Camera::getProjection() const {
    return mProjection;
}

Matrix4 Camera::getViewProjection() const {
    return mView * mProjection;
}

void Camera::setPosition(const Vector3& pos) {
    transform().setPosition(pos);
}

Vector3 Camera::getPosition() const {
    return transform().getPosition();
}

void Camera::lookAt(const Vector3 & position) {
    mLookAt = position;
}

Vector3 Camera::screenToWorldPoint(const Vector2 & position, float z) {
    //各行列の逆行列を算出
    auto invView = mView;
    auto invProj = mProjection;
    auto viewport = Matrix4::identity;
    invView.inverse();
    invProj.inverse();
    viewport.m[0][0] = Window::width() / 2.f;
    viewport.m[1][1] = -Window::height() / 2.f;
    viewport.m[3][0] = Window::width() / 2.f;
    viewport.m[3][1] = Window::height() / 2.f;
    viewport.inverse();

    //逆変換
    auto temp = viewport * invProj * invView;
    Vector3 out = Vector3::zero;
    //out = Vector3(position, z) * temp;

    return out;
}

bool Camera::viewFrustumCulling(const Vector3& pos, float radius) const {
    //ワールド空間からビュー空間に変換
    auto viewPos = Vector3::transform(pos, mView);
    //事前計算
    auto aspect = static_cast<float>(Window::width()) / static_cast<float>(Window::height());
    auto calcX = mFarClip * Math::tan(mFOV / 2.f) * aspect;
    auto calcY = mFarClip * Math::tan(mFOV / 2.f);

    Vector3 p1, p2, p3;
    //左
    p1 = Vector3::zero;
    p2.x = -calcX;
    p2.y = -calcY;
    p2.z = mFarClip;
    p3 = p2;
    p3.y = -p2.y;
    auto left = Plane(p1, p2, p3);
    //右
    p1 = Vector3::zero;
    p2.x = calcX;
    p2.y = calcY;
    p2.z = mFarClip;
    p3 = p2;
    p3.y = -p2.y;
    auto right = Plane(p1, p2, p3);
    //上
    p1 = Vector3::zero;
    p2.x = -calcX;
    p2.y = calcY;
    p2.z = mFarClip;
    p3 = p2;
    p3.x = -p2.x;
    auto top = Plane(p1, p2, p3);
    //下
    p1 = Vector3::zero;
    p2.x = calcX;
    p2.y = -calcY;
    p2.z = mFarClip;
    p3 = p2;
    p3.x = -p2.x;
    auto bottom = Plane(p1, p2, p3);

    //6面と境界球判定
    //手前
    if (viewPos.z + radius < mNearClip) {
        return false;
    }
    //奥
    if (viewPos.z - radius > mFarClip) {
        return false;
    }
    //左
    auto dis = (viewPos.x * left.a) + (viewPos.z * left.c);
    if (dis > radius) {
        return false;
    }
    //右
    dis = (viewPos.x * right.a) + (viewPos.z * right.c);
    if (dis > radius) {
        return false;
    }
    //上
    dis = (viewPos.y * top.b) + (viewPos.z * top.c);
    if (dis > radius) {
        return false;
    }
    //下
    dis = (viewPos.y * bottom.b) + (viewPos.z * bottom.c);
    if (dis > radius) {
        return false;
    }

    return true;
}

void Camera::calcLookAt() {
    auto pos = getPosition();
    Vector3 zaxis = Vector3::normalize(mLookAt - pos);
    Vector3 xaxis = Vector3::normalize(Vector3::cross(transform().up(), zaxis));
    Vector3 yaxis = Vector3::normalize(Vector3::cross(zaxis, xaxis));
    Vector3 trans;
    trans.x = -Vector3::dot(xaxis, pos);
    trans.y = -Vector3::dot(yaxis, pos);
    trans.z = -Vector3::dot(zaxis, pos);

    float temp[4][4] = {
        { xaxis.x, yaxis.x, zaxis.x, 0.f },
        { xaxis.y, yaxis.y, zaxis.y, 0.f },
        { xaxis.z, yaxis.z, zaxis.z, 0.f },
        { trans.x, trans.y, trans.z, 1.f }
    };
    mView = Matrix4(temp);
}

void Camera::calcPerspectiveFOV(int width, int height) {
    float yScale = Math::cot(mFOV / 2.f);
    float xScale = yScale * static_cast<float>(height) / static_cast<float>(width);
    float temp[4][4] = {
        { xScale, 0.f, 0.f, 0.f },
        { 0.f, yScale, 0.f, 0.f },
        { 0.f, 0.f, mFarClip / (mFarClip - mNearClip), 1.f },
        { 0.f, 0.f, -mNearClip * mFarClip / (mFarClip - mNearClip), 0.f }
    };
    mProjection = Matrix4(temp);
}
