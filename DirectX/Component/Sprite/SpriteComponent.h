#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include <memory>
#include <string>

class Shader;
class Sprite;
class SpriteManager;
class Texture;
class Transform2D;

class SpriteComponent : public Component, public std::enable_shared_from_this<SpriteComponent> {
public:
    SpriteComponent(GameObject& gameObject);
    virtual ~SpriteComponent();
    virtual void awake() override;
    virtual void lateUpdate() override;
    virtual void finalize() override;
    virtual void onEnable(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    //描画
    void draw(const Matrix4& proj) const;

    //トランスフォーム
    Transform2D& transform() const;
    //色味
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
    //状態管理
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;
    //テクスチャの張替え
    void setTextureFromFileName(const std::string& fileName);
    //テクスチャを設定する
    void setTexture(const std::shared_ptr<Texture>& texture);
    //テクスチャ
    const Texture& texture() const;
    //シェーダーの取得
    const Shader& shader() const;
    //ファイル名の取得
    const std::string& fileName() const;
    //描画優先番号の取得
    int getDrawOrder() const;

    static void setSpriteManager(SpriteManager* manager);

private:
    void addToManager();

protected:
    std::unique_ptr<Sprite> mSprite;
    int mDrawOrder;

    static inline SpriteManager* mSpriteManager = nullptr;
};

