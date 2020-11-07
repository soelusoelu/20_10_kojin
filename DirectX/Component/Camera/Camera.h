#pragma once

#include "../Component.h"
#include "../../Collision/Collision.h"
#include "../../Math/Math.h"

class Camera : public Component {
public:
    Camera(GameObject& gameObject);
    ~Camera();
    virtual void awake() override;
    virtual void lateUpdate() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawInspector() override;
    const Matrix4& getView() const;
    const Matrix4& getProjection() const;
    // View * Projection
    Matrix4 getViewProjection() const;
    //位置の設定
    void setPosition(const Vector3& pos);
    //位置の取得
    Vector3 getPosition() const;
    //視点を指定
    void lookAt(const Vector3& position);
    //スクリーン座標をワールド座標に変換する
    //zが0のときカメラから最も近い点、1のとき最も遠い点を計算する z[0, 1]
    Vector3 screenToWorldPoint(const Vector2& position, float z = 1.f);
    //カメラ位置からスクリーン座標からワールド座標に変換した点へのレイを取得する
    //zが0のときカメラから最も近い点、1のとき最も遠い点を計算する z[0, 1]
    Ray screenToRay(const Vector2& position, float z = 1.f);
    //視錐台カリング
    //true : 視錐台の内側
    //false : 視錐台の外側
    bool viewFrustumCulling(const Vector3& pos, float radius) const;

private:
    void calcLookAt();
    void calcPerspectiveFOV(int width, int height);

private:
    Vector3 mLookAt;

    float mFOV;
    float mNearClip;
    float mFarClip;

    Matrix4 mView;
    Matrix4 mProjection;
};

