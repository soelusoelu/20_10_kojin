#pragma once

#include "IMeshLoader.h"
#include "../Math/Math.h"
#include <fbxsdk.h>
#include <memory>
#include <string>
#include <vector>

class FBX : public IMeshLoader {
public:
    FBX();
    ~FBX();
    virtual void perse(const std::string& fileName, std::vector<MeshVertices>& meshes) override;
    virtual const std::vector<unsigned short>& getIndices(unsigned meshIndex) const override;
    virtual const Material& getMaterial(unsigned index) const override;
    virtual unsigned getMeshCount() const override;

private:
    //メッシュ作成
    void createMesh(MeshVertices& meshVertices, FbxMesh* mesh, unsigned meshIndex);

    //現状使用してない(サンプル確認用)
    void loadPosition(FbxMesh* mesh, unsigned meshIndex);
    void loadNormal(FbxMesh* mesh, unsigned meshIndex);
    void loadUV(FbxMesh* mesh, unsigned meshIndex);

    //頂点情報 & 添字読み込み
    void loadFace(MeshVertices& meshVertices, FbxMesh* mesh, unsigned meshIndex);
    //添字計算
    void computeIndices(FbxMesh* mesh, unsigned meshIndex);

    //マテリアル読み込み
    void loadMaterial(FbxMesh* mesh, unsigned meshIndex);
    void loadMaterialAttribute(FbxSurfaceMaterial* material, unsigned meshIndex);
    void loadLambert(const FbxSurfaceLambert* lambert, unsigned meshIndex);
    void loadPhong(const FbxSurfacePhong* phong, unsigned meshIndex);

    //テクスチャ読み込み
    void loadTextures(FbxSurfaceMaterial* material, unsigned meshIndex);
    void createTexture(const FbxSurfaceMaterial* material, const char* type, unsigned meshIndex);
    FbxFileTexture* getFbxTexture(const FbxProperty& prop) const;

private:
    unsigned mNumMeshes;

    std::vector<std::vector<unsigned short>> mIndices;
    std::vector<Material> mMaterials;
};
