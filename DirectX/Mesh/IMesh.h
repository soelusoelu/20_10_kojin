#pragma once

#include "IMeshLoader.h"
#include "Material.h"
#include "../Collision/Collision.h"
#include "../Math/Math.h"

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
    //指定のインデックスバッファを取得する
    virtual const Indices& getMeshIndices(unsigned index) const = 0;
    //指定のメッシュのポリゴン数を取得する
    virtual unsigned getPolygonCount(unsigned index) const = 0;
    //指定のメッシュの指定のポリゴンを取得する
    virtual Triangle getPolygon(unsigned meshIndex, unsigned polygonIndex) const = 0;
    //指定のメッシュの指定のポリゴンにワールド行列を演算し取得する
    virtual Triangle getPolygon(unsigned meshIndex, unsigned polygonIndex, const Matrix4& world) const = 0;
};
