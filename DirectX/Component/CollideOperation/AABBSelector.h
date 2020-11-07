#pragma once

#include "../Component.h"
#include <memory>
#include <vector>

class AABBMouseScaler;
class AABBCollider;
class Camera;
class MeshComponent;

class AABBSelector : public Component {
    using AABBColliderPtr = std::shared_ptr<AABBCollider>;
    using AABBColliderPtrArray = std::vector<AABBColliderPtr>;

public:
    AABBSelector(GameObject& gameObject);
    ~AABBSelector();
    virtual void start() override;
    virtual void update() override;
    //メッシュからAABBを抜き出す
    void setAABBsFromMesh(const MeshComponent& mesh);
    //アクセスできる状態か
    bool accessable() const;

private:
    AABBSelector(const AABBSelector&) = delete;
    AABBSelector& operator=(const AABBSelector&) = delete;

    //マウスでAABBを選択する
    bool selectAABB();

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<AABBMouseScaler> mAABBMouseScaler;
    AABBColliderPtrArray mColliders;
    //このクラスにアクセスしてもいい状態か
    bool mCanAccess;
};
