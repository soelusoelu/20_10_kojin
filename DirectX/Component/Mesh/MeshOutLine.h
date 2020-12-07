#pragma once

#include "IDrawBefore.h"
#include "../Component.h"
#include "../../Math/Math.h"
#include "../../Mesh/IMesh.h"
#include "../../Mesh/IMeshDrawer.h"
#include <memory>

class Shader;
class SkinMeshComponent;
class Camera;
class DirectionalLight;

class MeshOutLine : public Component, public IDrawBefore {
public:
    MeshOutLine(GameObject& gameObject);
    ~MeshOutLine();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawInspector() override;

    //メッシュ描画前描画
    virtual void drawBefore(const Camera& camera, const DirectionalLight& dirLight) const override;

    //アウトラインの色を設定する
    void setOutLineColor(const Vector3& color);
    //アウトラインの色を取得する
    const Vector3& getOutLineColor() const;
    //アウトラインの太さを設定する
    void setOutLineThickness(float thickness);
    //アウトラインの太さを取得する
    float getOutLineThickness() const;
    //アウトラインを描画するか設定する
    void setActiveOutLine(bool value);
    //アウトラインを描画するか取得する
    bool getActiveOutLine() const;

private:
    MeshOutLine(const MeshOutLine&) = delete;
    MeshOutLine& operator=(const MeshOutLine&) = delete;

    //アウトラインを描画する
    void drawOutLine(const Camera& camera, const DirectionalLight& dirLight) const;

protected:
    const IMesh* mMesh;
    const IMeshDrawer* mDrawer;
    std::shared_ptr<Shader> mOutLineShader;
    std::shared_ptr<SkinMeshComponent> mSkinMesh;
    Vector3 mOutLineColor;
    float mOutLineThickness;
    bool mIsDrawOutLine;
    bool mIsAnimation;
    float mOffset;
};
