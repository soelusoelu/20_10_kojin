#pragma once

#include <memory>
#include <vector>

class Collider;

class Physics {
    using CollPtr = std::shared_ptr<Collider>;
    using CollPtrArray = std::vector<CollPtr>;

public:
    Physics();
    ~Physics();
    //コライダーの追加・削除
    void add(const CollPtr& collider);
    void remove(const CollPtr& collider);
    //全削除
    void clear();
    //総当たり判定
    void sweepAndPrune();

private:
    CollPtrArray mColliders;
};
