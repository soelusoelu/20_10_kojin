#pragma once

#include "Material.h"
#include "Vertex.h"
#include "../Math/Math.h"
#include <string>
#include <vector>

//頂点構造体の配列を別名定義
using MeshVertices = std::vector<MeshVertex>;

class IMeshLoader {
public:
    virtual ~IMeshLoader() = default;
    virtual void perse(const std::string& fileName, std::vector<MeshVertices>& meshes) = 0;
    virtual const std::vector<unsigned short>& getIndices(unsigned meshIndex) const = 0;
    virtual const Material& getMaterial(unsigned index) const = 0;
    virtual unsigned getMeshCount() const = 0;
};
