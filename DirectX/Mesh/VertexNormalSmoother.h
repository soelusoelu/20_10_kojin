#pragma once

#include "IMeshLoader.h"
#include <unordered_map>
#include <vector>

//頂点法線を平滑化するクラス
class VertexNormalSmoother {
    using SmoothData = std::unordered_map<unsigned short, std::vector<unsigned short>>;

public:
    VertexNormalSmoother();
    ~VertexNormalSmoother();

    //頂点法線の平均によるエッジの平滑化行う
    void smoothing(
        std::vector<MeshVertices>& meshesVertices,
        const std::vector<Indices>& meshesIndices
    );

private:
    VertexNormalSmoother(const VertexNormalSmoother&) = delete;
    VertexNormalSmoother& operator=(const VertexNormalSmoother&) = delete;

    //平滑化し法線を格納する
    void calculateSmoothedVertexNormals(
        MeshVertices& meshVertices,
        const Indices& indices,
        const SmoothData& smoothData
    );
};
