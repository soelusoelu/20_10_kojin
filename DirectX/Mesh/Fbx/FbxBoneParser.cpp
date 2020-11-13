#include "FbxBoneParser.h"
#include "FbxBoneWeightParser.h"
#include "FbxUtility.h"

FbxBoneParser::FbxBoneParser()
    : mBoneWeightParser(std::make_unique<FbxBoneWeightParser>())
{
}

FbxBoneParser::~FbxBoneParser() = default;

void FbxBoneParser::parse(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Bone>& bones,
    const std::vector<FbxMesh*>& fbxMeshes
) {
    for (int i = 0; i < fbxMeshes.size(); ++i) {
        //ボーンウェイトの読み込み
        mBoneWeightParser->parse(meshesVertices[i], fbxMeshes[i]);
    }

    //スキン情報の有無
    int skinCount = fbxMeshes[0]->GetDeformerCount(FbxDeformer::eSkin);
    if (skinCount <= 0) {
        return;
    }

    for (int i = 0; i < skinCount; ++i) {
        //i番目のスキンを取得
        FbxDeformer* fbxDeformer = fbxMeshes[0]->GetDeformer(i, FbxDeformer::eSkin);
        FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxDeformer);

        //ボーンを読み込んでいく
        loadBone(bones, fbxSkin);

        //親子付け
        setParentChildren(bones, fbxSkin);
    }
}

FbxCluster* FbxBoneParser::getFbxCluster(
    int index
) const {
    return mFbxClusterArray[index];
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
    mFbxClusterArray.resize(boneCount);

    //ボーンの数だけ読み込んでいく
    for (int i = 0; i < boneCount; ++i) {
        //i番目のボーンを取得
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
        mFbxClusterArray[i] = fbxCluster;

        auto& bone = bones[i];
        //ボーンにパラメータを設定する
        setBoneParameter(bone, fbxCluster);

        //セットに登録
        mBoneMap.emplace(bone.name, &bone);
    }
}

void FbxBoneParser::setBoneParameter(Bone& bone, FbxCluster* fbxCluster) {
    //ノードを取得
    FbxNode* fbxNode = fbxCluster->GetLink();

    //ボーン名取得
    bone.name = fbxNode->GetName();

    //モデルの初期姿勢を取得する
    FbxAMatrix linkMatrix;
    fbxCluster->GetTransformLinkMatrix(linkMatrix);
    bone.initMat = FbxUtility::substitutionMatrix(linkMatrix);

    //初期姿勢からオフセット行列を計算する
    bone.offsetMat = Matrix4::inverse(bone.initMat);
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
