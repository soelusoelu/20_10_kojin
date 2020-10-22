#pragma once

#include "LineRenderer.h"
#include "../../Math/Math.h"
#include <list>
#include <memory>

class Shader;
class Transform2D;

class LineRenderer2D : public LineRenderer {
    struct Line2DVertex {
        Vector2 pos;
    };

    struct Line2DParam {
        Vector2 p1;
        Vector2 p2;
        Vector3 color;
    };

public:
    LineRenderer2D();
    ~LineRenderer2D();

    virtual void clear() override;

    //描画したいライン情報を指定する
    void renderLine(const Vector2& p1, const Vector2& p2, const Vector3& color);

private:
    LineRenderer2D(const LineRenderer2D&) = delete;
    LineRenderer2D& operator=(const LineRenderer2D&) = delete;

    virtual unsigned getParamSize() const override;
    virtual const void* getVertexData() const override;
    virtual void createShader() override;
    virtual void drawLines(const Matrix4& proj) const override;

    //実際にラインを描画する
    void drawLine(const Line2DParam& param, const Matrix4& proj) const;

private:
    std::unique_ptr<Shader> mShader;
    std::list<Line2DParam> mLines;
    std::unique_ptr<Transform2D> mTransform;
};
