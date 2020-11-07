#include "FbxBoneParser.h"
#include "FbxAnimationTime.h"

FbxBoneParser::FbxBoneParser() :
    mAnimationTime(std::make_unique<FbxAnimationTime>()) {
}

FbxBoneParser::~FbxBoneParser() = default;

void FbxBoneParser::parse(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Bone>& bones,
    FbxScene* fbxScene
) {
    //アニメーション時間を取得する
    mAnimationTime->parse(fbxScene);

    //FbxMeshの数を取得
    auto numMeshes = fbxScene->GetSrcObjectCount<FbxMesh>();

    //シーンからメッシュを取得する
    FbxMesh* fbxMesh = fbxScene->GetSrcObject<FbxMesh>();

    //スキン情報の有無
    int skinCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
    if (skinCount <= 0) {
        return;
    }

    for (int i = 0; i < skinCount; ++i) {
        //i番目のスキンを取得
        FbxDeformer* fbxDeformer = fbxMesh->GetDeformer(i, FbxDeformer::eSkin);
        FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxDeformer);

        //ボーンを読み込んでいく
        loadBone(bones, fbxSkin);

        //親子付け
        setParentChildren(bones, fbxSkin);
    }
}

void FbxBoneParser::loadBone(
    std::vector<Bone>& bones,
    FbxSkin* fbxSkin
) {
    //ボーンの数を取得
    int boneCount = fbxSkin->GetClusterCount();
    if (boneCount <= 0) {
        return;
    }

    //ボーンの数に合わせて拡張
    bones.resize(boneCount);

    //ボーンの数だけ読み込んでいく
    for (int i = 0; i < boneCount; ++i) {
        //i番目のボーンを取得
        FbxCluster* bone = fbxSkin->GetCluster(i);

        //キーフレーム読み込み
        loadKeyFrames(bones[i], bone);

        //セットに登録
        mBoneMap.emplace(bones[i].name, &bones[i]);
    }
}

void FbxBoneParser::loadKeyFrames(
    Bone& bone,
    FbxCluster* fbxCluster
) {
    //ノードを取得
    FbxNode* fbxNode = fbxCluster->GetLink();

    //ボーン名取得
    bone.name = fbxNode->GetName();

    //フレーム数を取得
    bone.numFrame = mAnimationTime->getStopFrame();

    //モデルの初期姿勢を取得する
    FbxAMatrix linkMatrix;
    fbxCluster->GetTransformLinkMatrix(linkMatrix);
    bone.initMat = substitutionMatrix(linkMatrix);

    //初期姿勢からオフセット行列を計算する
    bone.offsetMat = Matrix4::inverse(bone.initMat);

    //フレーム数分拡張しとく
    bone.frameMat.resize(bone.numFrame);

    //フレーム数分フレーム時姿勢を取得する
    for (int j = 0; j < bone.numFrame; ++j) {
        //指定フレームでの時間を取得する
        auto time = mAnimationTime->getTime(j);
        //指定フレームでの姿勢
        bone.frameMat[j] = substitutionMatrix(fbxNode->EvaluateGlobalTransform(time));
    }
}

Matrix4 FbxBoneParser::substitutionMatrix(
    const FbxMatrix& src
) const {
    Matrix4 dst;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            dst.m[i][j] = static_cast<float>(src.mData[i][j]);
        }
    }

    return dst;
}

void FbxBoneParser::setParentChildren(
    std::vector<Bone>& bones,
    FbxSkin* fbxSkin
) {
    //親子付け
    for (size_t i = 0; i < mBoneMap.size(); i++) {
        //i番目のボーンを取得
        FbxNode* node = fbxSkin->GetCluster(i)->GetLink();

        //スケルトンを取得
        //FbxSkeleton* skeleton = node->GetSkeleton();
        ////スケルトンタイプを取得
        //FbxSkeleton::EType type = skeleton->GetSkeletonType();

        ////ノードじゃなければ終了
        //if (type != FbxSkeleton::eLimbNode) {
        //    return;
        //}

        //親ノードを取得
        FbxNode* parentNode = node->GetParent();
        //親の名前を取得
        std::string parentName = parentNode->GetName();

        //親の名前が登録されてなかったらルート
        auto itr = mBoneMap.find(parentName);
        //親がいるなら登録
        if (itr != mBoneMap.end()) {
            bones[i].parent = itr->second;
            itr->second->children.emplace_back(&bones[i]);
        }
    }

    Bone* root = nullptr;
    for (size_t i = 0; i < bones.size(); i++) {
        if (!bones[i].parent) {
            root = &bones[i];
            break;
        }
    }
    calcRelativeMatrix(*root, nullptr);
}

void FbxBoneParser::calcRelativeMatrix(
    Bone& me,
    const Matrix4* parentOffset
) {
    for (size_t i = 0; i < me.children.size(); i++) {
        calcRelativeMatrix(*me.children[i], &me.offsetMat);
    }
    if (parentOffset) {
        me.initMat *= *parentOffset;
    }
}
