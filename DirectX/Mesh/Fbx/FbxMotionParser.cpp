#include "FbxMotionParser.h"
#include "FbxAnimationTime.h"
#include "FbxBoneParser.h"
#include "FbxUtility.h"

FbxMotionParser::FbxMotionParser()
    : mAnimationTime(std::make_unique<FbxAnimationTime>())
    , mBoneParser(std::make_unique<FbxBoneParser>())
{
}

FbxMotionParser::~FbxMotionParser() = default;

void FbxMotionParser::parse(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Motion>& motions,
    std::vector<Bone>& bones,
    FbxScene* fbxScene,
    const std::vector<FbxMesh*>& fbxMeshes
) {
    //アニメーション時間を取得する
    mAnimationTime->parse(motions, mMotionsTime, fbxScene);

    //ボーンの読み込み
    mBoneParser->parse(meshesVertices, bones, fbxMeshes);

    //モーション数 * ボーン数だけキーフレームを読み込む
    for (int i = 0; i < motions.size(); ++i) {
        auto& motion = motions[i];
        auto& motionTime = mMotionsTime[i];
        //ボーン数分拡張しとく
        motion.frameMat.resize(bones.size());
        for (int j = 0; j < bones.size(); ++j) {
            //キーフレーム読み込み
            loadKeyFrames(motion, motionTime, mBoneParser->getFbxCluster(j)->GetLink(), j);
        }
    }
}

void FbxMotionParser::loadKeyFrames(
    Motion& motion,
    const FbxMotionTime& motionTime,
    FbxNode* fbxNode,
    int boneIndex
) {
    auto& frameMat = motion.frameMat[boneIndex];
    frameMat.resize(motion.numFrame);

    //フレーム数分フレーム時姿勢を取得する
    for (int i = 0; i < motion.numFrame; ++i) {
        //指定フレームでの時間を取得する
        const auto& time = motionTime.getTime(i);
        //指定フレームでの姿勢
        frameMat[i] = FbxUtility::substitutionMatrix(fbxNode->EvaluateGlobalTransform(time));
    }
}
