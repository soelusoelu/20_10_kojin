#pragma once

#include "../Mesh/IMeshLoader.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Shader;
class Mesh;
class TextureFromFile;

class AssetsManager {
public:
    AssetsManager();
    ~AssetsManager();
    std::unique_ptr<Shader> createShader(const std::string& fileName);
    std::shared_ptr<TextureFromFile> createTexture(const std::string& filePath);
    std::shared_ptr<TextureFromFile> createTextureFromModel(const std::string& fileName);
    std::shared_ptr<Mesh> createMesh(const std::string& filePath);
    std::unique_ptr<IMeshLoader> createMeshLoader(const std::string& filePath, std::vector<MeshVertices>& vertices);
    void setDataDirectory(const std::string& filePath) const;

private:
    AssetsManager(const AssetsManager&) = delete;
    AssetsManager& operator=(const AssetsManager&) = delete;

private:
    std::unordered_map<std::string, std::shared_ptr<TextureFromFile>> mTextures;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> mMeshes;

    static inline bool mInstantiated = false;
};
