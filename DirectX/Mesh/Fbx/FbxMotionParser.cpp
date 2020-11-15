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
    //アニメーション数を取得する
    auto numAnim = fbxScene->GetSrcObjectCount<FbxAnimStack>();
    //アニメーションがないなら終了
    if (numAnim <= 0) {
        return;
    }

    //モーション数を設定する
    motions.resize(numAnim);

    //ボーンの読み込み
    mBoneParser->parse(meshesVertices, bones, fbxMeshes);

    for (int i = 0; i < motions.size(); ++i) {
        //アニメーションを取得する
        FbxAnimStack* anim = fbxScene->GetSrcObject<FbxAnimStack>(i);
        //シーン切り替え
        fbxScene->SetCurrentAnimationStack(anim);

        auto& motion = motions[i];
        const auto& motionTime = mAnimationTime->getMotionTime(fbxScene, anim);

        //モーションの名前を設定
        motion.name = anim->GetName();

        //モーションのフレーム数を設定
        motion.numFrame = motionTime.stopFrame - motionTime.startFrame;

        //キーフレームの読み込み
        loadAllKeyFrames(motion, motionTime, bones.size());
    }
}

void FbxMotionParser::loadAllKeyFrames(
    Motion& motion,
    const FbxMotionTime& motionTime,
    unsigned boneCount
) {
    //ボーン数分拡張しとく
    motion.frameMat.resize(boneCount);
    for (unsigned j = 0; j < boneCount; ++j) {
        //キーフレーム読み込み
        loadKeyFrames(motion, motionTime, mBoneParser->getFbxCluster(j)->GetLink(), j);
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
