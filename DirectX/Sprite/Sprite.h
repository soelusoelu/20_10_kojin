#pragma once

#include "../GameObject/Object.h"
#include "../Math/Math.h"
#include <memory>
#include <string>

class Shader;
class SpriteManager;
class Texture;
class Transform2D;

class Sprite : public Object {
public:
    Sprite();
    Sprite(const std::string& fileName);
    virtual ~Sprite();
    //行列の計算をする
    void computeWorldTransform();
    //描画
    void draw(const Matrix4& proj) const;
    //Transform
    Transform2D& transform() const;
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
    //テクスチャサイズの取得
    const Vector2& getTextureSize() const;
    //アクティブ指定
    void setActive(bool value);
    //アクティブ状態の取得
    bool getActive() const;
    //ファイル名からテクスチャを設定する
    void setTextureFromFileName(const std::string& fileName);
    //テクスチャを設定する
    void setTexture(const std::shared_ptr<Texture>& texture);
    //テクスチャの取得
    const Texture& texture() const;
    //シェーダーの取得
    const Shader& shader() const;
    //ファイル名の取得
    const std::string& fileName() const;

private:
    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;

private:
    std::unique_ptr<Transform2D> mTransform;
    std::shared_ptr<Texture> mTexture;
    std::shared_ptr<Shader> mShader;
    Vector3 mColor;
    float mAlpha;
    Vector4 mUV;
    std::string mFileName;
    bool mIsActive;
};
