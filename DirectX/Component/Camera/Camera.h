#pragma once

#include "../Component.h"
#include "../../Math/Math.h"

class Camera : public Component {
public:
    Camera(GameObject& gameObject);
    ~Camera();
    virtual void awake() override;
    virtual void lateUpdate() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
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
    //スクリーン座標をワールド座標に変換
    Vector3 screenToWorldPoint(const Vector2& position, float z);
    //視錐台カリング
    //true: 視錐台の内側
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

