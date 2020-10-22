#include "Sprite.h"
#include "SpriteManager.h"
#include "../Device/AssetsManager.h"
#include "../DirectX/DirectXInclude.h"
#include "../System/World.h"
#include "../System/shader/ConstantBuffers.h"
#include "../System/shader/Shader.h"
#include "../System/Texture/TextureFromFile.h"
#include "../Transform/Transform2D.h"
#include <cassert>
#include <vector>

Sprite::Sprite() :
    mTransform(std::make_unique<Transform2D>()),
    mTexture(nullptr),
    mShader(World::instance().assetsManager().createShader("Texture.hlsl")),
    mColor(ColorPalette::white, 1.f),
    mUV(0.f, 0.f, 1.f, 1.f),
    mFileName(),
    mIsActive(true) {

    computeWorldTransform();
}

Sprite::Sprite(const std::string& fileName) :
    Sprite() {
    //テクスチャ生成
    setTextureFromFileName(fileName);
}

Sprite::~Sprite() = default;

void Sprite::computeWorldTransform() {
    if (getActive()) {
        mTransform->computeWorldTransform();
    }
}

void Sprite::draw(const Matrix4& proj) const {
    if (!mTexture) {
        return;
    }

    //シェーダーを登録
    mShader->setShaderInfo();
    //テクスチャーを登録
    mTexture->setTextureInfo();

    //シェーダーのコンスタントバッファーに各種データを渡す
    TextureConstantBuffer cb;
    cb.wp = mTransform->getWorldTransform() * proj;
    cb.color = mColor;
    cb.uv = mUV;

    //シェーダーにデータ転送
    mShader->transferData(&cb, sizeof(cb));

    //プリミティブをレンダリング
    DirectX::instance().drawIndexed(6);
}

Transform2D& Sprite::transform() const {
    return *mTransform;
}

void Sprite::setColor(const Vector3 & color) {
    mColor.x = color.x;
    mColor.y = color.y;
    mColor.z = color.z;
}

void Sprite::setColor(float r, float g, float b) {
    mColor.x = r;
    mColor.y = g;
    mColor.z = b;
}

void Sprite::setAlpha(float alpha) {
    mColor.w = alpha;
}

const Vector4& Sprite::getColor() const {
    return mColor;
}

void Sprite::setUV(float l, float t, float r, float b) {
    assert(0.f <= l || l <= 1.f);
    assert(0.f <= t || t <= 1.f);
    assert(l <= r || r <= 1.f);
    assert(t <= b || b <= 1.f);

    mUV.x = l;
    mUV.y = t;
    mUV.z = r;
    mUV.w = b;

    //サイズ修正
    const auto& texSize = mTexture->getTextureSize();
    Vector2 size = Vector2(texSize.x * (r - l), texSize.y * (b - t));

    //テクスチャサイズを変更したことを通知
    mTransform->setSize(size);
}

const Vector4& Sprite::getUV() const {
    return mUV;
}

const Vector2& Sprite::getTextureSize() const {
    return mTexture->getTextureSize();
}

void Sprite::setActive(bool value) {
    mIsActive = value;
}

bool Sprite::getActive() const {
    return mIsActive;
}

void Sprite::setTextureFromFileName(const std::string& fileName) {
    if (mTexture) {
        mTexture.reset();
    }
    mTexture = World::instance().assetsManager().createTexture(fileName);

    //Transformに通知
    mTransform->setSize(mTexture->getTextureSize());

    //ファイル名変更
    mFileName = fileName;
}

void Sprite::setTexture(const std::shared_ptr<Texture>& texture) {
    if (mTexture) {
        mTexture.reset();
    }
    mTexture = texture;

    //Transformに通知
    mTransform->setSize(mTexture->getTextureSize());

    //ファイル名変更
    mFileName.clear();
}

const Texture& Sprite::texture() const {
    return *mTexture;
}

const Shader& Sprite::shader() const {
    return *mShader;
}

const std::string& Sprite::fileName() const {
    return mFileName;
}
