#pragma once

#include "Collider.h"
#include "../../Collision/Collision.h"
#include "../../Math/Math.h"
#include "../../Mesh/IMesh.h"
#include "../../Mesh/IMeshLoader.h"

class SphereCollider : public Collider {
public:
    SphereCollider(GameObject& gameObject);
    ~SphereCollider();
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void drawInspector() override;

    //中心位置と半径を直接設定する
    void set(const Vector3& center, float radius);
    //球を取得する
    const Sphere& getSphere() const;

private:
    //メッシュから球を作る
    void createSphere(const IMesh& mesh);
    //メッシュから最小、最大点を割り出す
    void computeCenterRadius(Vector3& outMin, Vector3& outMax, const MeshVertices& meshVertices);

private:
    Sphere mSphere;
    Vector3 mDefaultCenter;
    float mDefaultRadius;
};
