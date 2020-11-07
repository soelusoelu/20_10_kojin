#pragma once

#include "../Component.h"
#include "../../Collision/Collision.h"
#include "../../Math/Math.h"
#include <memory>
#include <vector>

class Camera;
class MeshComponent;
class AABBCollider;

//キャラクターをマウスで操作するクラス
class DragAndDropCharacter : public Component {
public:
    DragAndDropCharacter(GameObject& gameObject);
    ~DragAndDropCharacter();
    virtual void start() override;
    virtual void update() override;
    virtual void drawInspector() override;

private:
    DragAndDropCharacter(const DragAndDropCharacter&) = delete;
    DragAndDropCharacter& operator=(const DragAndDropCharacter&) = delete;

    //カメラからマウスでクリックした地点へのレイを取得する
    Ray getRayCamraToMousePos(const Vector2& mousePos) const;
    //すべての地形メッシュとレイの衝突判定を行う
    bool intersectRayGroundMeshes(const Ray& ray);
    //マウスでメッシュを選択する
    void selectMesh(const Ray& ray);
    //メッシュを衝突点まで移動させる
    void moveToIntersectPoint();

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<AABBCollider> mAABB;
    //キャラクターを立たせたい地形メッシュ配列
    std::vector<std::shared_ptr<MeshComponent>> mGroundMeshes;
    //レイと地形との衝突点
    Vector3 mIntersectPoint;
    //地形メッシュとレイが衝突したか
    bool mIsIntersectRayGround;
    //メッシュを選択中か
    bool mSelectedMesh;
};
