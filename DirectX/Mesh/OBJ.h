#pragma once

#include "IMeshLoader.h"
#include "Material.h"
#include "../Math/Math.h"
#include <fstream>
#include <string>
#include <vector>

class OBJ : public IMeshLoader {
public:
    OBJ();
    ~OBJ();
    virtual void parse(
        const std::string& filePath,
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Indices>& meshesIndices,
        std::vector<Material>& materials,
        std::vector<Motion>& motions,
        std::vector<Bone>& bones
    ) override;

private:
    //頂点情報関連
    void loadPosition(std::istringstream& iss);
    void loadNormal(std::istringstream& iss);
    void loadUV(std::istringstream& iss);
    void loadFace(
        MeshVertices& meshVertices,
        Indices& indices,
        std::istringstream& iss
    );

    //マテリアル関連
    void loadMaterial(
        Material& material,
        std::istringstream& iss,
        const std::string& directoryPath
    );
    void loadMaterialName(Material& material, std::istringstream& iss);
    void loadAmbient(Material& material, std::istringstream& iss);
    void loadDiffuse(Material& material, std::istringstream& iss);
    void loadSpecular(Material& material, std::istringstream& iss);
    void loadTexture(Material& material, std::istringstream& iss, const std::string& directoryPath);

    //不要な行をスキップする
    bool isSkip(const std::string& line);

private:
    std::vector<Vector3> mPositions;
    std::vector<Vector3> mNormals;
    std::vector<Vector2> mUVs;
};
