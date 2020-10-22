#include "LineRenderer.h"
#include "../../DirectX/DirectXInclude.h"

LineRenderer::LineRenderer() :
    mVertexBuffer(nullptr),
    mIndexBuffer(nullptr) {
}

LineRenderer::~LineRenderer() = default;

void LineRenderer::draw(const Matrix4& proj) const {
    //描画共通処理は最初に済ませる
    //バーテックスバッファーを登録
    mVertexBuffer->setVertexBuffer();
    //インデックスバッファーを登録
    mIndexBuffer->setIndexBuffer();

    //描画指令を出す
    drawLines(proj);
}

void LineRenderer::initialize() {
    //バーテックスバッファの作成
    createVertexBuffer();
    //インデックスバッファの作成
    createIndexBuffer();

    //子クラスのシェーダーを作成する
    createShader();
}

void LineRenderer::createVertexBuffer() {
    BufferDesc bd;
    bd.oneSize = getParamSize();
    bd.size = bd.oneSize * 2; //頂点が2つだから
    bd.usage = Usage::USAGE_IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);

    SubResourceDesc sub;
    sub.data = getVertexData();

    mVertexBuffer = std::make_unique<VertexBuffer>(bd, sub);
}

void LineRenderer::createIndexBuffer() {
    static constexpr unsigned short indices[] = {
        0, 1
    };
    BufferDesc bd;
    bd.size = sizeof(indices);
    bd.usage = Usage::USAGE_IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_INDEX);

    SubResourceDesc sub;
    sub.data = indices;

    mIndexBuffer = std::make_unique<IndexBuffer>(bd, sub);
}
