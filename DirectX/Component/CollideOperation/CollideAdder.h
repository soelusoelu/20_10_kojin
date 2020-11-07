#pragma once

#include "../Component.h"

class MeshComponent;

//コライダーを追加するクラス
class CollideAdder : public Component {
public:
    CollideAdder(GameObject& gameObject);
    ~CollideAdder();
    void addAABBCollide(MeshComponent& mesh);

private:
    CollideAdder(CollideAdder&) = delete;
    CollideAdder& operator=(const CollideAdder&) = delete;
};
