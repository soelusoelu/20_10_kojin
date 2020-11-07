#pragma once

#include "../Bone.h"
#include "../IMeshLoader.h"
#include "../../Math/Math.h"
#include <fbxsdk.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class FbxAnimationTime;

//FBXボーン解析クラス
class FbxBoneParser {
public:
    FbxBoneParser();
    ~FbxBoneParser();

    //ボーンを解析する
    void parse(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Bone>& bones,
        FbxScene* fbxScene
    );

private:
    //ボーン読み込み
    void loadBone(
        std::vector<Bone>& bones,
        FbxSkin* fbxSkin
    );

    //キーフレーム読み込み
    void loadKeyFrames(
        Bone& bone,
        FbxCluster* fbxCluster
    );

    //FbxMatirxからMatrix4へ変換する
    Matrix4 substitutionMatrix(
        const FbxMatrix& src
    ) const;

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
    std::unique_ptr<FbxAnimationTime> mAnimationTime;
    //ボーンの名前から検索できるように
    std::unordered_map<std::string, Bone*> mBoneMap;
};
