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
    //不透明度(0～1、1が不透明, 0が透明)
    void setAlpha(float alpha);
    const Vector4& getColor() const;
    //切り取り範囲(left, top, right, bottom, 0～1)
    void setUV(float l, float t, float r, float b);
    const Vector4& getUV() const;
    //テクスチャサイズの取得
    const Vector2& getTextureSize() const;
    //アクティブ指定
    void setActive(bool value);
    //アクティブ状態の取得
    bool getActive() const;
    //テクスチャを設定する
    void setTextureFromFileName(const std::string& fileName);
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
    std::unique_ptr<Shader> mShader;
    Vector4 mColor;
    Vector4 mUV;
    std::string mFileName;
    bool mIsActive;
};
