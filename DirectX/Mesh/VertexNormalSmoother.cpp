#include "VertexNormalSmoother.h"

VertexNormalSmoother::VertexNormalSmoother() = default;

VertexNormalSmoother::~VertexNormalSmoother() = default;

void VertexNormalSmoother::smoothing(
    std::vector<MeshVertices>& meshesVertices,
    const std::vector<Indices>& meshesIndices
) {
    for (size_t i = 0; i < meshesVertices.size(); ++i) {
        auto& meshVertices = meshesVertices[i];
        const auto& indices = meshesIndices[i];

        SmoothData smoothData;
        for (size_t j = 0; j < meshVertices.size(); ++j) {
            smoothData[indices[j]].emplace_back(static_cast<unsigned short>(j));
        }

        calculateSmoothedVertexNormals(meshVertices, indices, smoothData);
    }
}

void VertexNormalSmoother::calculateSmoothedVertexNormals(
    MeshVertices& meshVertices,
    const Indices& indices,
    const SmoothData& smoothData
) {
    for (auto itr = smoothData.begin(); itr != smoothData.end(); ++itr) {
        //各面用の共通頂点コレクション
        const auto& v = itr->second;

        //全頂点の法線を平均する
        auto normal = Vector3::zero;
        for (const auto& index : v) {
            normal += meshVertices[index].normal;
        }

        //正規化する
        normal = Vector3::normalize(normal / static_cast<float>(v.size()));

        //共通法線を使用するすべての頂点データに書き込む
        for (const auto& index : v) {
            meshVertices[index].normal = normal;
        }
    }
}
