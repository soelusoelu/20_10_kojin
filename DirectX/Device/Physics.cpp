#include "Physics.h"
#include "../Collision/Collision.h"
#include "../Component/ComponentManager.h"
#include "../Component/Collider/Collider.h"
#include "../Component/Collider/SphereCollider.h"
#include <algorithm>

Physics::Physics() {
    Collider::setPhysics(this);
}

Physics::~Physics() {
    Collider::setPhysics(nullptr);
}

void Physics::add(const CollPtr& collider) {
    mColliders.emplace_back(collider);
}

void Physics::remove(const CollPtr& collider) {
    auto itr = std::find(mColliders.begin(), mColliders.end(), collider);
    if (itr != mColliders.end()) {
        std::iter_swap(itr, mColliders.end() - 1);
        mColliders.pop_back();
    }
}

void Physics::clear() {
    mColliders.clear();
}

void Physics::sweepAndPrune() {
    if (mColliders.empty()) {
        return;
    }

    //center.x - mCenter.radiusが小さい順にソート
    //std::sort(mColliders.begin(), mColliders.end(), [](CollPtr a, CollPtr b) {
    //    auto circleA = std::dynamic_pointer_cast<SphereCollider>(a);
    //    auto circleB = std::dynamic_pointer_cast<SphereCollider>(b);
    //    return circleA->getSphere().center.x - circleA->getSphere().radius < circleB->getSphere().center.x - circleB->getSphere().radius;
    //});

    //for (size_t i = 0; i < mColliders.size(); i++) {
    //    auto a = std::dynamic_pointer_cast<SphereCollider>(mColliders[i]);
    //    if (!a->getEnable()) {
    //        continue;
    //    }
    //    const auto& as = a->getSphere();
    //    //mCircles[i]の中心+半径を取得
    //    float max = as.center.x + as.radius;
    //    for (size_t j = i + 1; j < mColliders.size(); j++) {
    //        auto b = std::dynamic_pointer_cast<SphereCollider>(mColliders[j]);
    //        if (!b->getEnable()) {
    //            continue;
    //        }
    //        const auto& bs = b->getSphere();
    //        //もしmCircles[j]の中心-半径が、mCircles[i]の中心+半径を超えていたら、
    //        //mCircles[i]と交差する可能性があるボックスは存在しない
    //        if (bs.center.x - bs.radius > max) {
    //            break;
    //        } else if (Intersect::intersectSphere(as, bs)) {
    //            a->addHitCollider(b);
    //            b->addHitCollider(a);
    //        }
    //    }
    //}
}
