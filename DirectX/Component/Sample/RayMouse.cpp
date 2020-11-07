#include "RayMouse.h"
#include "../Camera/Camera.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../Collision/Collision.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"
#include "../../Transform/Transform3D.h"
#include "../../System/Window.h"
#include "../../Utility/StringUtil.h"

RayMouse::RayMouse(GameObject& gameObject) :
    Component(gameObject),
    mCamera(nullptr),
    mMesh(nullptr),
    mAABB(nullptr),
    mIntersectPoint(Vector3::zero),
    mSelectedMesh(false)
{
}

RayMouse::~RayMouse() = default;

void RayMouse::start() {
    mCamera = gameObject().getGameObjectManager().find("Camera")->componentManager().getComponent<Camera>();
    mMesh = getComponent<MeshComponent>();
    mAABB = getComponent<AABBCollider>();
    transform().setPivot(Vector3::down);
}

void RayMouse::update() {
    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();

    //ワールド座標におけるマウスの位置を取得
    auto mousePos = mouse.getMousePosition();
    mousePos.clamp(Vector2::zero, Vector2(Window::width(), Window::height()));
    auto clickPos = mCamera->screenToWorldPoint(mousePos, 1.f);

    //計算に必要な要素を用意しておく
    const auto& cameraPos = mCamera->getPosition();
    Vector3 dir = Vector3::normalize(clickPos - cameraPos);
    Ray ray(cameraPos, dir);
    Plane plane(Vector3::up, Vector3::zero);

    //マウスの左ボタンが押されたら
    if (mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        //メッシュとレイの衝突判定
        //if (!Intersect::intersectRayMesh(ray, mMesh->getMesh(), transform(), mIntersectPoint)) {
        //    return;
        //}
        //無限平面とレイの衝突判定
        if (!Intersect::intersectRayPlane(ray, plane, mIntersectPoint)) {
            return;
        }
        //AABBとレイの衝突判定
        if (Intersect::intersectRayAABB(ray, mAABB->getAABB())) {
            mSelectedMesh = true;
        }
    } else if (mouse.getMouseButton(MouseCode::LeftButton)) {
        Intersect::intersectRayPlane(ray, plane, mIntersectPoint);
    } else if (mouse.getMouseButtonUp(MouseCode::LeftButton)) {
        mSelectedMesh = false;
    }

    if (mSelectedMesh) {
        transform().setPosition(mIntersectPoint);
    }
}
