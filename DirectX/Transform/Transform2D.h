#pragma once

#include "Pivot.h"
#include "../Math/Math.h"

class Transform2D {
public:
    Transform2D();
    ~Transform2D();

    //ワールド行列更新
    bool computeWorldTransform();
    //ワールド行列を取得する
    const Matrix4& getWorldTransform() const;

    //ピクセル単位で位置指定
    void setPosition(const Vector2& pos);
    const Vector2& getPosition() const;
    void translate(const Vector2& translation);
    void translate(float x, float y);

    //回転
    void setRotation(float angle);
    float getRotation() const;
    void rotate(float angle);

    //ピボット位置
    void setPivot(Pivot pivot);
    const Vector2& getPivot() const;
    Pivot getPivotEnum() const;

    //倍率で拡縮
    void setScale(const Vector2& scale);
    void setScale(float scale);
    const Vector2& getScale() const;

    //ポリゴンサイズ
    void setSize(const Vector2& size);
    const Vector2& getSize() const;

    //インスペクター
    void drawInspector();

private:
    Transform2D(const Transform2D&) = delete;
    Transform2D& operator=(const Transform2D&) = delete;

    void shouldRecomputeTransform();

private:
    Matrix4 mWorldTransform;
    Vector2 mPosition;
    float mRotation;
    Pivot mPivot;
    Vector2 mPivotPosition;
    Vector2 mScale;
    Vector2 mSize;
    bool mIsRecomputeTransform;
};

