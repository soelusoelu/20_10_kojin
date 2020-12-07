#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include "../../Mesh/IAnimation.h"
#include "../../Mesh/IMesh.h"
#include "../../Mesh/IMeshDrawer.h"
#include "../../Mesh/Material.h"
#include <memory>
#include <string>
#include <vector>

class Mesh;

class MeshComponent : public Component {
    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

public:
    MeshComponent(GameObject& gameObject);
    virtual ~MeshComponent();
    virtual void awake() override;
    virtual void start() override;
    virtual void finalize() override;
    virtual void onEnable(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawInspector() override;

    //メッシュを作成する
    void createMesh(const std::string& fileName, const std::string& directoryPath);

    //描画できる状態か
    bool isDraw() const;

    //状態
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;

    //メッシュインターフェースを取得する
    const IMesh* getMesh() const;
    //アニメーションインターフェースを取得する
    IAnimation* getAnimation() const;
    //描画インターフェースを取得する
    const IMeshDrawer* getDrawer() const;

    //使用する色の割合を設定する
    void setColorRatio(const Vector3& color);
    //使用する色の割合を取得する
    const Vector3& getColorRatio() const;
    //アルファ値を設定する
    void setAlpha(float alpha);
    //アルファ値を取得する
    float getAlpha() const;

private:
    MeshComponent(const MeshComponent&) = delete;
    MeshComponent& operator=(const MeshComponent&) = delete;

protected:
    std::shared_ptr<Mesh> mMesh;
    std::string mFileName;
    std::string mDirectoryPath;
    State mState;
    Vector3 mColor;
    float mAlpha;
};
