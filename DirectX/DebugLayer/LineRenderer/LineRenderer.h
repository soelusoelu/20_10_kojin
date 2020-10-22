#pragma once

#include "../../Math/Math.h"
#include <memory>

class VertexBuffer;
class IndexBuffer;

class LineRenderer {
public:
    LineRenderer();
    virtual ~LineRenderer();
    void draw(const Matrix4& proj) const;
    void initialize();

private:
    LineRenderer(const LineRenderer&) = delete;
    LineRenderer& operator=(const LineRenderer&) = delete;

    //ラインのパラメータサイズを取得する
    virtual unsigned getParamSize() const = 0;
    //ラインの頂点情報を取得する
    virtual const void* getVertexData() const = 0;
    //シェーダーを作成する
    virtual void createShader() = 0;
    //溜まっているライン情報を削除する
    virtual void clear() = 0;
    //すべてのラインを描画していく
    virtual void drawLines(const Matrix4& proj) const = 0;

    //バーテックスバッファを作成する
    void createVertexBuffer();
    //インデックスバッファを作成する
    void createIndexBuffer();

protected:
    std::unique_ptr<VertexBuffer> mVertexBuffer;
    std::unique_ptr<IndexBuffer> mIndexBuffer;
};
