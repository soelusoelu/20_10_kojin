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

    //ビュー行列を取得する
    const Matrix4& getView() const;
    //プロジェクション行列を取得する
    const Matrix4& getProjection() const;
    // View * Projection
    Matrix4 getViewProjection() const;
    //位置を設定する
    void setPosition(const Vector3& pos);
    //位置を取得する
    Vector3 getPosition() const;
    //視点を設定する
    void lookAt(const Vector3& position);
    //視点を取得する
    const Vector3& getLookAt() const;
    //視野角を設定する
    void setFov(float fov);
    //視野角を取得する
    float getFov() const;
    //ニアクリップを設定する
    void setNearClip(float nearClip);
    //ニアクリップを取得する
    float getNearClip() const;
    //ファークリップを設定する
    void setFarClip(float farClip);
    //ファークリップを取得する
    float getFarClip() const;

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
    void calcView();
    void calcProj();

private:
    Vector3 mLookAt;

    float mFOV;
    float mNearClip;
    float mFarClip;

    Matrix4 mView;
    Matrix4 mProjection;
};

