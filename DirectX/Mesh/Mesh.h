#pragma once

#include "Bone.h"
#include "IAnimation.h"
#include "IMesh.h"
#include "IMeshDrawer.h"
#include "IMeshLoader.h"
#include "Material.h"
#include "Motion.h"
#include <memory>
#include <string>
#include <vector>

class VertexBuffer;
class IndexBuffer;

class Mesh : public IMesh, public IAnimation, public IMeshDrawer {
public:
    Mesh();
    ~Mesh();

    //指定のマテリアルの取得
    virtual const Material& getMaterial(unsigned index) const override;
    //メッシュの数を取得する
    virtual unsigned getMeshCount() const override;
    //指定の頂点情報を取得
    virtual const MeshVertices& getMeshVertices(unsigned index) const override;
    //指定のインデックスバッファを取得する
    virtual const Indices& getMeshIndices(unsigned index) const override;
    //指定のメッシュのポリゴン数を取得する
    virtual unsigned getPolygonCount(unsigned index) const override;
    //指定のメッシュの指定のポリゴンを取得する
    virtual Triangle getPolygon(unsigned meshIndex, unsigned polygonIndex) const override;
    //指定のメッシュの指定のポリゴンにワールド行列を演算し取得する
    virtual Triangle getPolygon(unsigned meshIndex, unsigned polygonIndex, const Matrix4& world) const override;

    //モーションを取得する
    virtual const Motion& getMotion(unsigned index) const override;
    //モーション数を取得する
    virtual unsigned getMotionCount() const override;
    //モーション名を設定する
    virtual void setMotionName(const std::string& name, unsigned index) override;
    //ボーンを取得する
    virtual const Bone& getBone(unsigned index) const override;
    //ボーン数を取得する
    virtual unsigned getBoneCount() const override;

    //メッシュを描画する
    virtual void draw(unsigned meshIndex) const override;

    //ファイル名からメッシュを生成する
    void loadMesh(const std::string& filePath);

private:
    //初期化処理
    void initialize(const std::string& filePath);
    //メッシュを生成する
    void createMesh(const std::string& filePath);
    //バーテックスバッファを生成する
    void createVertexBuffer(unsigned meshIndex);
    //インデックスバッファを生成する
    void createIndexBuffer(unsigned meshIndex);

private:
    std::unique_ptr<IMeshLoader> mMesh;
    std::vector<MeshVertices> mMeshesVertices;
    std::vector<Indices> mMeshesIndices;
    std::vector<Material> mMaterials;
    std::vector<Motion> mMotions;
    std::vector<Bone> mBones;
    std::vector<std::unique_ptr<VertexBuffer>> mVertexBuffers;
    std::vector<std::unique_ptr<IndexBuffer>> mIndexBuffers;
};
