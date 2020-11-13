#pragma once

#include "FbxMotionTime.h"
#include "../Bone.h"
#include "../IMeshLoader.h"
#include "../Motion.h"
#include "../../Math/Math.h"
#include <fbxsdk.h>
#include <string>
#include <unordered_map>
#include <vector>

class FbxBoneWeightParser;

//FBXボーン解析クラス
class FbxBoneParser {
public:
    FbxBoneParser();
    ~FbxBoneParser();

    //ボーンを解析する
    void parse(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Bone>& bones,
        const std::vector<FbxMesh*>& fbxMeshes
    );

    //FbxClusterを取得する
    FbxCluster* getFbxCluster(
        int index
    ) const;

private:
    //ボーン読み込み
    void loadBone(
        std::vector<Bone>& bones,
        FbxSkin* fbxSkin
    );

    //ボーンのパラメータを設定する
    void setBoneParameter(
        Bone& bone,
        FbxCluster* fbxCluster
    );

    //ボーンの親子付け
    void setParentChildren(
        std::vector<Bone>& bones,
        FbxSkin* fbxSkin
    );

    //初期姿勢を親の姿勢からの相対姿勢に直す
    void calcRelativeMatrix(
        Bone& me,
        const Matrix4* parentOffset
    );

private:
    //ウェイト読み込み
    std::unique_ptr<FbxBoneWeightParser> mBoneWeightParser;
    //ボーンの名前から検索できるように
    std::unordered_map<std::string, Bone*> mBoneMap;
    //クラスター配列
    std::vector<FbxCluster*> mFbxClusterArray;
};
