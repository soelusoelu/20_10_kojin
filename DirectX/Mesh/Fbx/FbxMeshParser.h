#pragma once

#include "../IMeshLoader.h"
#include <fbxsdk.h>
#include <vector>

//FBXメッシュ解析クラス
class FbxMeshParser {
public:
    FbxMeshParser();
    ~FbxMeshParser();

    //メッシュを解析する
    void parse(
        MeshVertices& meshVertices,
        Indices& indices,
        FbxMesh* fbxMesh
    ) const;

private:
    //現状使用してない(サンプル確認用)
    void loadNormal(FbxMesh* mesh);
    void loadUV(FbxMesh* mesh);

    //UV配列を取得する
    void getUVs(
        FbxArray<FbxVector2>& uvs,
        const FbxMesh* fbxMesh
    ) const;

    //UV配列を接線配列を取得する
    void getUVsAndTangents(
        FbxArray<FbxVector2>& uvs,
        FbxLayerElementArrayTemplate<FbxVector4>* tangents,
        FbxMesh* fbxMesh
    ) const;

    //UV名前リストからUV配列を作成する
    void getUVsFromUVSetNameList(
        FbxArray<FbxVector2>& uvs,
        FbxStringList& uvNameList,
        const FbxMesh* fbxMesh
    ) const;
};
