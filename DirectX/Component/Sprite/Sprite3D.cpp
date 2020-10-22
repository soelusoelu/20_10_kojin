#include "Sprite3D.h"
#include "../../DebugLayer/Debug.h"
#include "../../Device/AssetsManager.h"
#include "../../DirectX/DirectX.h"
#include "../../GameObject/GameObject.h"
#include "../../Sprite/SpriteManager.h"
#include "../../System/Window.h"
#include "../../System/World.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../System/Shader/Shader.h"
#include "../../System/Texture/Texture.h"
#include "../../System/Texture/TextureFromFile.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"
#include <cassert>
#include <vector>

Sprite3D::Sprite3D(GameObject& gameObject) :
    Component(gameObject),
    mTransform(std::make_unique<Transform3D>()),
    mTexture(nullptr),
    mShader(nullptr),
    mTextureAspect(Vector2::zero),
    mColor(Vector4(1.f, 1.f, 1.f, 1.f)),
    mUV(Vector4(0.f, 0.f, 1.f, 1.f)),
    mFileName(""),
    mIsActive(true),
    mIsBillboard(false) {
}

Sprite3D::~Sprite3D() = default;

void Sprite3D::awake() {
    if (mFileName.empty()) {
        Debug::logWarning(getComponentName() + ": No filename.");
        return;
    }

    mTexture = World::instance().assetsManager().createTexture(mFileName);
    mShader = World::instance().assetsManager().createShader("Texture.hlsl");

    //テクスチャのアスペクト比を計算
    const auto& texSize = mTexture->getTextureSize();
    auto v = texSize.y / texSize.x;
    mTextureAspect.set(1.f, v);
    mTransform->setScale(mTransform->getScale() * Vector3(mTextureAspect, 1.f));

    addToManager();

    setActive(gameObject().getActive());
}

void Sprite3D::lateUpdate() {
    if (getActive()) {
        mTransform->computeWorldTransform();
    }
}

void Sprite3D::finalize() {
    destroy();
}

void Sprite3D::onEnable(bool value) {
    setActive(value);
}

void Sprite3D::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "fileName", &mFileName);
    JsonHelper::getBool(inObj, "isActive", &mIsActive);
    JsonHelper::getBool(inObj, "isBillboard", &mIsBillboard);
    Vector3 vec3;
    if (JsonHelper::getVector3(inObj, "position", &vec3)) {
        mTransform->setPosition(vec3);
    }
    if (JsonHelper::getVector3(inObj, "rotation", &vec3)) {
        mTransform->rotate(vec3);
    }
    Vector2 vec2;
    if (JsonHelper::getVector2(inObj, "scale", &vec2)) {
        mTransform->setScale(Vector3(vec2, 1.f));
    }
    if (JsonHelper::getVector3(inObj, "color", &vec3)) {
        setColor(vec3);
    }
    float alpha;
    if (JsonHelper::getFloat(inObj, "alpha", &alpha)) {
        setAlpha(alpha);
    }
    JsonHelper::getVector4(inObj, "uv", &mUV);
}

void Sprite3D::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FileName", mFileName);
    inspect->emplace_back("Position", mTransform->getPosition());
    inspect->emplace_back("Rotation", mTransform->getRotation().euler());
    inspect->emplace_back("Scale", mTransform->getScale());
}

void Sprite3D::draw(const Matrix4& viewProj) const {
    //シェーダーを登録
    mShader->setShaderInfo();
    //テクスチャーを登録
    mTexture->setTextureInfo();

    //シェーダーのコンスタントバッファーに各種データを渡す
    TextureConstantBuffer cb;
    cb.wp = mTransform->getWorldTransform() * viewProj;
    cb.color = mColor;
    cb.uv = mUV;

    //シェーダーにデータ転送
    mShader->transferData(&cb, sizeof(cb));

    //プリミティブをレンダリング
    DirectX::instance().drawIndexed(6);
}

void Sprite3D::drawBillboard(const Matrix4& invView, const Matrix4& viewProj) {
    //シェーダーを登録
    mShader->setShaderInfo();
    //テクスチャーを登録
    mTexture->setTextureInfo();

    //シェーダーのコンスタントバッファーに各種データを渡す
    TextureConstantBuffer cb;
    //ワールド、射影行列を渡す
    cb.wp = invView * mTransform->getWorldTransform() * viewProj;
    cb.color = mColor;
    cb.uv = mUV;

    //シェーダーにデータ転送
    mShader->transferData(&cb, sizeof(cb));

    //プリミティブをレンダリング
    DirectX::instance().drawIndexed(6);
}

Transform3D& Sprite3D::transform() const {
    return *mTransform;
}

void Sprite3D::setColor(const Vector3& color) {
    mColor.x = color.x;
    mColor.y = color.y;
    mColor.z = color.z;
}

void Sprite3D::setColor(float r, float g, float b) {
    mColor.x = r;
    mColor.y = g;
    mColor.z = b;
}

void Sprite3D::setAlpha(float alpha) {
    mColor.w = alpha;
}

const Vector4& Sprite3D::getColor() const {
    return mColor;
}

void Sprite3D::setUV(float l, float t, float r, float b) {
    assert(0.f <= l || l <= 1.f);
    assert(0.f <= t || t <= 1.f);
    assert(l <= r || r <= 1.f);
    assert(t <= b || b <= 1.f);

    mUV.x = l;
    mUV.y = t;
    mUV.z = r;
    mUV.w = b;
}

const Vector4& Sprite3D::getUV() const {
    return mUV;
}

void Sprite3D::setActive(bool value) {
    mIsActive = value;
}

bool Sprite3D::getActive() const {
    return mIsActive;
}

const Texture& Sprite3D::texture() const {
    return *mTexture;
}

const Shader& Sprite3D::shader() const {
    return *mShader;
}

const std::string& Sprite3D::fileName() const {
    return mFileName;
}

void Sprite3D::setBillboard(bool value) {
    mIsBillboard = value;
}

bool Sprite3D::isBillboard() const {
    return mIsBillboard;
}

void Sprite3D::setSpriteManager(SpriteManager* manager) {
    mSpriteManager = manager;
}

void Sprite3D::addToManager() {
    if (mSpriteManager) {
        mSpriteManager->add3D(shared_from_this());
    }
}
