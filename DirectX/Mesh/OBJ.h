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
    virtual void perse(const std::string& fileName, std::vector<MeshVertices>& meshes) override;
    virtual const std::vector<unsigned short>& getIndices(unsigned meshIndex) const override;
    virtual const Material& getMaterial(unsigned index) const override;
    virtual unsigned getMeshCount() const override;

private:
    //頂点情報関連
    void loadPosition(std::istringstream& iss);
    void loadNormal(std::istringstream& iss);
    void loadUV(std::istringstream& iss);
    void loadFace(std::istringstream& iss, MeshVertices& meshVertices);

    //マテリアル関連
    void loadMaterial(std::istringstream& iss);
    void loadMaterialName(std::istringstream& iss);
    void loadAmbient(std::istringstream& iss);
    void loadDiffuse(std::istringstream& iss);
    void loadSpecular(std::istringstream& iss);
    void loadTexture(std::istringstream& iss);

    //不要な行をスキップする
    bool isSkip(const std::string& line);

private:
    std::vector<Vector3> mPositions;
    std::vector<Vector3> mNormals;
    std::vector<Vector2> mUVs;
    std::vector<unsigned short> mIndices;
    std::vector<Material> mMaterials;
};
