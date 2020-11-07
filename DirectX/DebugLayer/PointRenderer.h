#pragma once

#include "../Math/Math.h"
#include <list>
#include <memory>

class Transform3D;
class Shader;

class PointRenderer {
    struct Point3DParam {
        Vector3 point;
        Vector3 color;
    };

public:
    PointRenderer();
    ~PointRenderer();
    //描画
    void draw(const Matrix4& viewProj) const;
    //初期化
    void initialize();
    //溜め込んである点情報を削除する
    void clear();
    //描画したいポイント情報を指定する
    void renderPoint(const Vector3& point, const Vector3& color);

private:
    PointRenderer(const PointRenderer&) = delete;
    PointRenderer& operator=(const PointRenderer&) = delete;

    //点を描画する
    void drawPoint(const Point3DParam& param, const Matrix4& viewProj) const;

private:
    std::list<Point3DParam> mPoints;
    std::unique_ptr<Transform3D> mTransform;
    std::unique_ptr<Shader> mShader;
};
