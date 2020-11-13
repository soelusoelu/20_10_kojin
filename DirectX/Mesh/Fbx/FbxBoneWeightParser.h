#pragma once

#include "../Bone.h"
#include "../IMeshLoader.h"
#include <fbxsdk.h>

//FBXボーンウェイト解析クラス
class FbxBoneWeightParser {
public:
    FbxBoneWeightParser();
    ~FbxBoneWeightParser();

    void parse(
        MeshVertices& meshVertices,
        const FbxMesh* fbxMesh
    );

private:
    FbxBoneWeightParser(const FbxBoneWeightParser&) = delete;
    FbxBoneWeightParser& operator=(const FbxBoneWeightParser&) = delete;

    //新しいウェイトを格納する
    void addWeight(
        MeshVertex& vertex,
        int boneIndex,
        float weight
    );

    //頂点ウェイトを正規化する
    void normalizeWeight(
        MeshVertices& meshVertice
    );

private:
    static constexpr int MAX_INFLUENCE = 4;
};
