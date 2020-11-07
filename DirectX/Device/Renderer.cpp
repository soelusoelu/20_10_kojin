#include "Renderer.h"
#include "../DirectX/DirectXInclude.h"
#include "../System/GBuffer.h"
#include "../System/Window.h"
#include "../System/Texture/Texture.h"
#include "../Utility/LevelLoader.h"

Renderer::Renderer() :
    mGBuffer(std::make_unique<GBuffer>()) {
}

Renderer::~Renderer() = default;

void Renderer::initialize() {
    mGBuffer->create();
}

void Renderer::renderToTexture() {
    MyDirectX::DirectX::instance().setViewport(Window::standardWidth(), Window::standardHeight());
    mGBuffer->renderToTexture();
}

void Renderer::renderFromTexture(const Camera& camera, const LightManager& lightManager) {
    MyDirectX::DirectX::instance().setViewport(Window::width(), Window::height());
    mGBuffer->renderFromTexture(camera, lightManager);
}

void Renderer::renderMesh() const {
    auto& dx = MyDirectX::DirectX::instance();
    //ビューポートの設定
    dx.setViewport(Window::width(), Window::height());
    //プリミティブ・トポロジーをセット
    dx.setPrimitive(PrimitiveType::TRIANGLE_LIST);
    //デプステスト有効化
    dx.depthStencilState()->depthTest(true);
    //デプスマスク有効化
    dx.depthStencilState()->depthMask(true);
    //半透明合成
    dx.blendState()->translucent();
}

void Renderer::renderPoint3D() const {
    auto& dx = MyDirectX::DirectX::instance();
    //ビューポートの設定
    dx.setViewport(Window::width(), Window::height());
    //プリミティブ・トポロジーをセット
    dx.setPrimitive(PrimitiveType::POINT_LIST);
    //半透明合成
    dx.blendState()->translucent();
    //デプステスト有効化
    dx.depthStencilState()->depthTest(true);
    dx.depthStencilState()->depthMask(true);
}

void Renderer::renderLine2D(Matrix4* proj) const {
    //原点をスクリーン左上にするために平行移動
    proj->m[3][0] = -1.f;
    proj->m[3][1] = 1.f;
    //ピクセル単位で扱うために
    proj->m[0][0] = 2.f / Window::width();
    proj->m[1][1] = -2.f / Window::height();

    //デプステスト無効化
    MyDirectX::DirectX::instance().depthStencilState()->depthTest(false);
}

void Renderer::renderLine3D() const {
    auto& dx = MyDirectX::DirectX::instance();
    //ビューポートの設定
    dx.setViewport(Window::width(), Window::height());
    //プリミティブ・トポロジーをセット
    dx.setPrimitive(PrimitiveType::LINE_LIST);
    //半透明合成
    dx.blendState()->translucent();
    //デプステスト有効化
    dx.depthStencilState()->depthTest(true);
    dx.depthStencilState()->depthMask(true);
}

void Renderer::renderSprite() const {
    auto& dx = MyDirectX::DirectX::instance();

    //ビューポートの設定
    dx.setViewport(Window::width(), Window::height());
    //プリミティブ・トポロジーをセット
    dx.setPrimitive(PrimitiveType::TRIANGLE_LIST);
    //インデックスバッファーをセット
    Texture::indexBuffer->setIndexBuffer();
    //半透明合成
    dx.blendState()->translucent();
    //カリングオフ
    dx.rasterizerState()->setCulling(CullMode::NONE);
}

void Renderer::renderSprite2D(Matrix4* proj) const {
    //原点をスクリーン左上にするために平行移動
    proj->m[3][0] = -1.f;
    proj->m[3][1] = 1.f;
    //ピクセル単位で扱うために
    proj->m[0][0] = 2.f / Window::width();
    proj->m[1][1] = -2.f / Window::height();

    //バーテックスバッファーをセット
    Texture::vertexBuffer->setVertexBuffer();
    //デプステスト無効化
    MyDirectX::DirectX::instance().depthStencilState()->depthTest(false);
}

void Renderer::renderSprite3D() const {
    //バーテックスバッファーをセット
    Texture::vertexBuffer3D->setVertexBuffer();
    //デプステスト有効化
    auto& dx = MyDirectX::DirectX::instance();
    dx.depthStencilState()->depthTest(true);
    dx.depthStencilState()->depthMask(true);
}

void Renderer::renderToDebug(Matrix4* proj) const {
    auto& dx = MyDirectX::DirectX::instance();

    dx.setDebugRenderTarget();
    dx.setViewport(Window::debugWidth(), Window::debugHeight());

    //原点をスクリーン左上にするために平行移動
    proj->m[3][0] = -1.f;
    proj->m[3][1] = 1.f;
    //ピクセル単位で扱うために
    proj->m[0][0] = 2.f / Window::width();
    proj->m[1][1] = -2.f / Window::height();

    //バーテックスバッファーをセット
    Texture::vertexBuffer->setVertexBuffer();
    //デプステスト無効化
    dx.depthStencilState()->depthTest(false);
}

void Renderer::renderPointLight() const {
    mGBuffer->setShaderResources();
}
