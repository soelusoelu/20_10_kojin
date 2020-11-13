#pragma once

#include "FbxMotionTime.h"
#include "../IMeshLoader.h"
#include "../Motion.h"
#include <fbxsdk.h>
#include <memory>
#include <vector>

class FbxAnimationTime;
class FbxBoneParser;

//FBXモーション解析クラス
class FbxMotionParser {
public:
    FbxMotionParser();
    ~FbxMotionParser();

    //モーションを読み込む
    void parse(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Motion>& motions,
        std::vector<Bone>& bones,
        FbxScene* fbxScene,
        const std::vector<FbxMesh*>& fbxMeshes
    );

private:
    FbxMotionParser(const FbxMotionParser&) = delete;
    FbxMotionParser& operator=(const FbxMotionParser&) = delete;

    //キーフレーム読み込み
    void loadKeyFrames(
        Motion& motion,
        const FbxMotionTime& motionTime,
        FbxNode* fbxNode,
        int boneIndex
    );

private:
    std::unique_ptr<FbxAnimationTime> mAnimationTime;
    std::unique_ptr<FbxBoneParser> mBoneParser;
    std::vector<FbxMotionTime> mMotionsTime;
};
