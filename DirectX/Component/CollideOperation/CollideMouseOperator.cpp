#include "CollideMouseOperator.h"
#include "AABBSelector.h"
#include "CollideAdder.h"
#include "MeshAdder.h"
#include "../Camera/Camera.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../Collision/Collision.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"

CollideMouseOperator::CollideMouseOperator(GameObject& gameObject)
    : Component(gameObject)
    , mCamera(nullptr)
    , mAABBSelector(nullptr)
    , mCollideAdder(nullptr)
    , mMeshAdder(nullptr)
    , mSelecteMesh(nullptr)
{
}

CollideMouseOperator::~CollideMouseOperator() = default;

void CollideMouseOperator::start() {
    const auto& gameObjectManager = gameObject().getGameObjectManager();
    mCamera = gameObjectManager.find("Camera")->componentManager().getComponent<Camera>();

    mAABBSelector = getComponent<AABBSelector>();
    mMeshAdder = getComponent<MeshAdder>();
    mCollideAdder = getComponent<CollideAdder>();

    //指定のタグを含んでいるオブジェクトをすべて取得する
    const auto& grounds = gameObjectManager.findGameObjects(GROUND_TAG);
    //取得したオブジェクトからメッシュを取得する
    for (const auto& g : grounds) {
        auto mesh = g->componentManager().getComponent<MeshComponent>();
        if (mesh) {
            mGroundMeshes.emplace_back(mesh);
        }
    }
}

void CollideMouseOperator::update() {
    //マウスの左ボタンを押した瞬間だったら
    if (Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        clickMouseLeftButton();
    }

    //コライダー追加するか
    if (Input::keyboard().getKeyDown(KeyCode::J)) {
        if (mSelecteMesh) {
            //選択してるメッシュにコライダーを追加する
            addCollider(*mSelecteMesh);
        }
    }

    //マウスの右ボタンを押した瞬間だったら
    if (Input::mouse().getMouseButtonDown(MouseCode::RightButton)) {
        addMesh();
    }
}

void CollideMouseOperator::clickMouseLeftButton() {
    if (mAABBSelector->accessable()) {
        //最もカメラから近いメッシュを選択する
        selectNearestMesh();
    }
}

void CollideMouseOperator::selectNearestMesh() {
    //地形とレイが衝突していなかったら終了
    if (!intersectRayGroundMeshes()) {
        return;
    }

    //メッシュに付随するAABBを送る
    mAABBSelector->setAABBsFromMesh(*mSelecteMesh);
}

bool CollideMouseOperator::intersectRayGroundMeshes() {
    //カメラからマウスの位置へ向かうレイを取得
    auto rayCameraToMousePos = mCamera->screenToRay(Input::mouse().getMousePosition());

    //すべての地形メッシュとレイの衝突判定
    Vector3 intersectPoint;
    for (const auto& gm : mGroundMeshes) {
        //今選択しているメッシュなら飛ばす
        if (gm == mSelecteMesh) {
            continue;
        }

        //メッシュとレイの衝突判定
        if (Intersect::intersectRayMesh(rayCameraToMousePos, gm->getMesh(), gm->transform(), intersectPoint)) {
            changeSelectMesh(gm);
            return true;
        }
    }

    //どれとも衝突しなかった
    return false;
}

void CollideMouseOperator::changeSelectMesh(const std::shared_ptr<MeshComponent>& mesh) {
    //今と同じメッシュなら終了
    if (mesh == mSelecteMesh) {
        return;
    }

    //選択対象を変更する
    mSelecteMesh = mesh;

    //選択中のメッシュ以外の当たり判定を非表示にする
    for (const auto& gm : mGroundMeshes) {
        const auto& aabbs = gm->getComponents<AABBCollider>();
        for (const auto& aabb : aabbs) {
            aabb->setRenderCollision(gm == mSelecteMesh);
        }
    }
}

void CollideMouseOperator::addMesh() {
    //新たにメッシュを追加する
    auto newMesh = mMeshAdder->addMeshCreateGameObject(GROUND_TAG);

    //生成に失敗したいたら終了
    if (!newMesh) {
        return;
    }

    //メッシュにコライダーを追加する
    addCollider(*newMesh);
    //このメッシュを選択対象にする
    changeSelectMesh(newMesh);
    //地形配列に追加する
    mGroundMeshes.emplace_back(newMesh);
}

void CollideMouseOperator::addCollider(MeshComponent& mesh) {
    //メッシュにコライダーを追加する
    mCollideAdder->addAABBCollide(mesh);
    //コライダーを追加したことを知らせるために
    mAABBSelector->setAABBsFromMesh(mesh);
}
