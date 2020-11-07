#pragma once

#include "../Component.h"
#include <memory>
#include <vector>

class Camera;
class MeshComponent;
class AABBSelector;
class MeshAdder;
class CollideAdder;

class CollideMouseOperator : public Component {
public:
    CollideMouseOperator(GameObject& gameObject);
    ~CollideMouseOperator();
    virtual void start() override;
    virtual void update() override;

private:
    CollideMouseOperator(const CollideMouseOperator&) = delete;
    CollideMouseOperator& operator=(const CollideMouseOperator&) = delete;

    //マウスの左ボタンを押した瞬間の処理
    void clickMouseLeftButton();
    //マウスでメッシュを選択する
    void selectNearestMesh();
    //すべての地形メッシュとレイの衝突判定を行う
    bool intersectRayGroundMeshes();
    //選択対象のメッシュを変更する
    void changeSelectMesh(const std::shared_ptr<MeshComponent>& mesh);
    //ファイルからメッシュを追加する
    void addMesh();
    //メッシュにコライダーを追加する
    void addCollider(MeshComponent& mesh);

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<AABBSelector> mAABBSelector;
    std::shared_ptr<MeshAdder> mMeshAdder;
    std::shared_ptr<CollideAdder> mCollideAdder;

    //全地形メッシュ配列
    std::vector<std::shared_ptr<MeshComponent>> mGroundMeshes;
    //メッシュが選択されているか
    std::shared_ptr<MeshComponent> mSelecteMesh;

    static constexpr const char* GROUND_TAG = "Ground";
};
