#include "DragAndDropCharacter.h"
#include "../Camera/Camera.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Imgui/imgui.h"
#include "../../Input/Input.h"
#include "../../Transform/Transform3D.h"
#include "../../System/Window.h"

DragAndDropCharacter::DragAndDropCharacter(GameObject& gameObject) :
    Component(gameObject),
    mCamera(nullptr),
    mAABB(nullptr),
    mIntersectPoint(Vector3::zero),
    mIsIntersectRayGround(false),
    mSelectedMesh(true) {
    //生成直後から動かしたいからmSelectedMeshはtrue
}

DragAndDropCharacter::~DragAndDropCharacter() = default;

void DragAndDropCharacter::start() {
    const auto& gameObjectManager = gameObject().getGameObjectManager();
    mCamera = gameObjectManager.find("Camera")->componentManager().getComponent<Camera>();
    mAABB = getComponent<AABBCollider>();

    //指定のタグを含んでいるオブジェクトをすべて取得する
    const auto& grounds = gameObjectManager.findGameObjects("Ground");
    //取得したオブジェクトからメッシュを取得する
    for (const auto& g : grounds) {
        auto mesh = g->componentManager().getComponent<MeshComponent>();
        if (mesh) {
            mGroundMeshes.emplace_back(mesh);
        }
    }

    //ピボットの位置を足元にするため
    transform().setPivot(Vector3::down);
}

void DragAndDropCharacter::update() {
    //フラグ初期化
    mIsIntersectRayGround = false;

    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();

    //カメラからマウスの位置へ向かうレイを取得
    auto rayCameraToMousePos = getRayCamraToMousePos(mouse.getMousePosition());

    //マウスの左ボタンを押した瞬間だったら
    if (mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        selectMesh(rayCameraToMousePos);
    }
    //マウスの左ボタンを押し続けていたら
    if (mouse.getMouseButton(MouseCode::LeftButton)) {
        intersectRayGroundMeshes(rayCameraToMousePos);
    }
    //マウスの左ボタンを離した瞬間だったら
    if (mouse.getMouseButtonUp(MouseCode::LeftButton)) {
        mSelectedMesh = false;
    }

    //条件を満たしていたら移動
    moveToIntersectPoint();
}

void DragAndDropCharacter::drawInspector() {
    ImGui::Checkbox("SelectedMesh", &mSelectedMesh);
}

Ray DragAndDropCharacter::getRayCamraToMousePos(const Vector2& mousePos) const {
    //マウス座標をゲームウィンドウ範囲でクランプする
    auto mp = mousePos.clamp(mousePos, Vector2::zero, Vector2(Window::width(), Window::height()));
    //ワールド座標におけるマウスの位置を取得
    auto worldPos = mCamera->screenToWorldPoint(mp, 1.f);

    //レイ作成
    const auto& cameraPos = mCamera->getPosition();
    Vector3 dir = Vector3::normalize(worldPos - cameraPos);
    Ray ray(cameraPos, dir);

    return ray;
}

bool DragAndDropCharacter::intersectRayGroundMeshes(const Ray& ray) {
    //すべての地形メッシュとレイの衝突判定
    for (const auto& gm : mGroundMeshes) {
        if (Intersect::intersectRayMesh(ray, gm->getMesh(), gm->transform(), mIntersectPoint)) {
            mIsIntersectRayGround = true;
            return true;
        }
    }

    //どれとも衝突しなかった
    return false;
}

void DragAndDropCharacter::selectMesh(const Ray& ray) {
    //地形とレイが衝突していなかったら終了
    if (!intersectRayGroundMeshes(ray)) {
        return;
    }

    //AABBとレイの衝突判定
    if (Intersect::intersectRayAABB(ray, mAABB->getAABB())) {
        //衝突していたらメッシュが選択される
        mSelectedMesh = true;
    }
}

void DragAndDropCharacter::moveToIntersectPoint() {
    //メッシュが選択されていなかったら終了
    if (!mSelectedMesh) {
        return;
    }
    //レイが地形と衝突していなかったら終了
    if (!mIsIntersectRayGround) {
        return;
    }

    //X軸を基準に移動制限を設ける
    auto movePoint = mIntersectPoint;
    auto offset = mAABB->getAABB().max.x - transform().getPosition().x;
    if (movePoint.x + offset > 0.f) {
        movePoint.x = -offset;
    }

    //衝突点まで移動
    transform().setPosition(movePoint);
}
