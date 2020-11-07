#include "FbxBoneWeightParser.h"

FbxBoneWeightParser::FbxBoneWeightParser() = default;

FbxBoneWeightParser::~FbxBoneWeightParser() = default;

void FbxBoneWeightParser::parse(
    MeshVertices& meshVertices,
    const FbxMesh* fbxMesh
) {
    FbxDeformer* fbxDeformer = fbxMesh->GetDeformer(0, FbxDeformer::eSkin);
    FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxDeformer);
    int boneCount = fbxSkin->GetClusterCount();
    for (int boneIndex = 0; boneIndex < boneCount; ++boneIndex) {
        FbxCluster* bone = fbxSkin->GetCluster(boneIndex);
        //影響する頂点の数
        int weightCount = bone->GetControlPointIndicesCount();
        //このボーンによって移動する頂点のインデックスの配列
        int* weightIndices = bone->GetControlPointIndices();
        //重み
        double* weights = bone->GetControlPointWeights();
        //頂点のインデックス
        int* polygonVertices = fbxMesh->GetPolygonVertices();

        for (int i = 0; i < weightCount; ++i) {
            int index = weightIndices[i];
            float weight = static_cast<float>(weights[i]);

            for (int j = 0; j < meshVertices.size(); ++j) {
                //頂点番号と一致するのを探す
                if (polygonVertices[j] != index) {
                    continue;
                }

                //頂点にウェイト情報を追加する
                addWeight(meshVertices[j], boneIndex, weight);
            }
        }
    }

    //ウェイト正規化
    //normalizeWeight(meshVertices);
}

void FbxBoneWeightParser::addWeight(
    MeshVertex& vertex,
    int boneIndex,
    float weight
) {
    //ウェイトを格納できる場所を探す
    int pos;
    if (!getInsertPos(vertex, pos)) {
        return;
    }

    //頂点情報にウェイトを追加
    vertex.index[pos] = boneIndex;
    vertex.weight[pos] = weight;
}

bool FbxBoneWeightParser::getInsertPos(
    const MeshVertex& vertex,
    int& pos
) {
    for (int i = 0; i < MAX_INFLUENCE; ++i) {
        //ウェイトが0なら格納できる
        if (Math::nearZero(vertex.weight[i])) {
            //ウェイトを格納できる位置を記録して終了
            pos = i;
            return true;
        }
    }

    //格納できる場所がない
    return false;
}

void FbxBoneWeightParser::normalizeWeight(
    MeshVertices& meshVertice
) {
    //5本以上にまたっがてる場合のため
    for (size_t i = 0; i < meshVertice.size(); i++) {
        auto& vertex = meshVertice[i];
        float sumWeight = 0.f;
        //頂点のウェイトの合計を求める
        for (int j = 0; j < MAX_INFLUENCE; ++j) {
            float weight = vertex.weight[j];
            if (weight <= 0.f) {
                break;
            }
            sumWeight += weight;
        }

        //ボーンから影響を受けないなら次へ
        if (sumWeight <= 0.f) {
            continue;
        }

        //正規化
        for (int j = 0; j < MAX_INFLUENCE; ++j) {
            vertex.weight[j] /= sumWeight;
        }
    }
}
