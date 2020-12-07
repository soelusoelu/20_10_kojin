#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include <memory>
#include <string>

class Shader;
class SpriteManager;
class Texture;
class TextureFromFile;
class Transform3D;

class Sprite3D : public Component, public std::enable_shared_from_this<Sprite3D> {
public:
    Sprite3D(GameObject& gameObject);
    ~Sprite3D();
    virtual void awake() override;
    virtual void lateUpdate() override;
    virtual void finalize() override;
    virtual void onEnable(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawInspector() override;

    //描画
    void draw(const Matrix4& viewProj) const;
    //ビルボードで描画
    void drawBillboard(const Matrix4& invView, const Matrix4& viewProj);
    //トランスフォーム
    Transform3D& transform() const;
    //色味 [0, 1]
    void setColor(const Vector3& color);
    void setColor(float r, float g, float b);
    //色を取得する
    const Vector3& getColor() const;
    //不透明度 [0, 1]
    void setAlpha(float alpha);
    //不透明度を取得する
    float getAlpha() const;
    //切り取り範囲(left, top, right, bottom, 0～1)
    void setUV(float l, float t, float r, float b);
    const Vector4& getUV() const;
    //状態管理
    void setActive(bool value);
    bool getActive() const;
    //ファイル名からテクスチャを設定する
    void setTextureFromFileName(const std::string& fileName);
    //テクスチャの取得
    const Texture& texture() const;
    //テクスチャのアスペクト比の取得
    const Vector2& getTextureAspect() const;
    //シェーダーの取得
    const Shader& shader() const;
    //ファイル名の取得
    const std::string& fileName() const;
    //ビルボードにするか
    void setBillboard(bool value);
    //ビルボードか
    bool isBillboard() const;

    //SpriteManagerの登録
    static void setSpriteManager(SpriteManager* manager);

protected:
    //マネージャークラスへの登録
    void addToManager();
    //テクスチャのアスペクト比を計算する
    void calcAspectRatio();

protected:
    std::unique_ptr<Transform3D> mTransform;
    std::shared_ptr<TextureFromFile> mTexture;
    std::shared_ptr<Shader> mShader;
    Vector2 mCurrentTextureSize;
    Vector2 mTextureAspect;
    Vector3 mColor;
    float mAlpha;
    Vector4 mUV;
    std::string mFileName;
    bool mIsActive;
    bool mIsBillboard;

    static inline SpriteManager* mSpriteManager = nullptr;
};
