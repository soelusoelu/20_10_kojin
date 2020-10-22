#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>

class GameObject;

class Transform3D : public std::enable_shared_from_this<Transform3D> {
    using TransformPtr = std::shared_ptr<Transform3D>;
    using TransformPtrList = std::list<TransformPtr>;

public:
    Transform3D(const std::shared_ptr<GameObject>& gameObject = nullptr);
    ~Transform3D();

    //アタッチ元のゲームオブジェクトを返す
    std::shared_ptr<GameObject> gameObject() const;

    //ワールド行列更新
    bool computeWorldTransform();
    //ワールド行列の取得
    const Matrix4& getWorldTransform() const;

    //位置の設定
    void setPosition(const Vector3& pos);
    //親子関係を考慮した位置の取得
    Vector3 getPosition() const;
    //ローカル位置の取得
    const Vector3& getLocalPosition() const;
    //translationの方向と距離に移動する
    void translate(const Vector3& translation);
    //x, y, z分移動する
    void translate(float x, float y, float z);

    //回転量の設定
    void setRotation(const Quaternion& rot);
    //軸を中心に回転量を設定
    void setRotation(const Vector3& axis, float angle);
    //親子関係を考慮した回転量の取得
    Quaternion getRotation() const;
    //ローカル回転量の取得
    const Quaternion& getLocalRotation() const;
    //軸を中心に回転させる
    void rotate(const Vector3& axis, float angle);
    //eulers量回転させる
    void rotate(const Vector3& eulers);

    //ピボット位置の設定
    void setPivot(const Vector3& pivot);
    //ピボット位置の取得
    const Vector3& getPivot() const;

    //スケール値の設定
    void setScale(const Vector3& scale);
    //均等に拡縮する
    void setScale(float scale);
    //親子関係を考慮したスケール値の取得
    Vector3 getScale() const;
    //ローカルスケール値の取得
    const Vector3& getLocalScale() const;

    //回転量に応じた前方ベクトルの取得
    Vector3 forward() const;
    //回転量に応じた上ベクトルの取得
    Vector3 up() const;
    //回転量に応じた右ベクトルの取得
    Vector3 right() const;

    //親子関係
    void addChild(const TransformPtr& child);
    void removeChild(const Transform3D& child);
    void removeChild(const std::string& tag);
    TransformPtr getChild(const std::string& tag) const;
    TransformPtrList getChildren() const;
    TransformPtr parent() const;
    TransformPtr root() const;
    size_t getChildCount() const;

    //ロード/セーブ
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const;

private:
    Transform3D(const Transform3D&) = delete;
    Transform3D& operator=(const Transform3D&) = delete;

    //親の設定
    void setParent(const TransformPtr& parent);
    //ワールド行列の計算が必要になった際のフラグ設定
    void shouldRecomputeTransform();

private:
    std::weak_ptr<GameObject> mGameObject;
    Matrix4 mWorldTransform;
    Vector3 mPosition;
    Quaternion mRotation;
    Vector3 mPivot;
    Vector3 mScale;
    TransformPtr mParent;
    TransformPtrList mChildren;
    bool mIsRecomputeTransform;
};
