#pragma once

#include "Bone.h"
#include "IMeshLoader.h"
#include "Material.h"
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
    //ボーン配列を取得する
    virtual const std::vector<Bone>& getBones() const = 0;
};
