#pragma once

#include "Bone.h"
#include "IMeshLoader.h"
#include "Material.h"
#include "Motion.h"
#include <vector>

//外部公開用メッシュインターフェース
class IMesh {
public:
    virtual ~IMesh() = default;
    //指定のマテリアルの取得
    virtual const Material& getMaterial(unsigned index) const = 0;
    //メッシュの数を取得する
    virtual unsigned getMeshCount() const = 0;
    //指定の頂点情報を取得
    virtual const MeshVertices& getMeshVertices(unsigned index) const = 0;
    //モーションを取得する
    virtual const Motion& getMotion(unsigned index) const = 0;
    //モーション数を取得する
    virtual unsigned getMotionCount() const = 0;
    //ボーンを取得する
    virtual const Bone& getBone(unsigned index) const = 0;
    //ボーン数を取得する
    virtual unsigned getBoneCount() const = 0;
};
