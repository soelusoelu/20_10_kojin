#include "AssetsManager.h"
#include "../DebugLayer/Debug.h"
#include "../Mesh/FBX.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/OBJ.h"
#include "../System/World.h"
#include "../System/Shader/Shader.h"
#include "../System/Texture/TextureFromFile.h"
#include "../Utility/Directory.h"
#include "../Utility/FileUtil.h"
#include <cassert>

AssetsManager::AssetsManager() {
    assert(!mInstantiated);
    mInstantiated = true;
}

AssetsManager::~AssetsManager() {
    mInstantiated = false;
}

std::unique_ptr<Shader> AssetsManager::createShader(const std::string & fileName) {
    World::instance().directory().setShaderDirectory();
    return std::make_unique<Shader>(fileName);
}

std::shared_ptr<TextureFromFile> AssetsManager::createTexture(const std::string & filePath) {
    std::shared_ptr<TextureFromFile> texture = nullptr;
    auto itr = mTextures.find(filePath);
    if (itr != mTextures.end()) { //既に読み込まれている
        texture = itr->second;
    } else { //初読み込み
        World::instance().directory().setTextureDirectory(filePath);
        auto fileName = FileUtil::getFileNameFromDirectry(filePath);
        texture = std::make_shared<TextureFromFile>(fileName);
        mTextures.emplace(filePath, texture);
    }
    return texture;
}

std::shared_ptr<TextureFromFile> AssetsManager::createTextureFromModel(const std::string& fileName) {
    std::shared_ptr<TextureFromFile> texture = nullptr;
    auto itr = mTextures.find(fileName);
    if (itr != mTextures.end()) { //既に読み込まれている
        texture = itr->second;
    } else { //初読み込み
        //モデルからテクスチャを生成したい場合、事前にディレクトリを移動させている必要がある
        texture = std::make_shared<TextureFromFile>(fileName);
        mTextures.emplace(fileName, texture);
    }
    return texture;
}

std::shared_ptr<Mesh> AssetsManager::createMesh(const std::string& filePath) {
    std::shared_ptr<Mesh> mesh = nullptr;
    auto itr = mMeshes.find(filePath);
    if (itr != mMeshes.end()) { //既に読み込まれている
        mesh = itr->second;
    } else { //初読み込み
        mesh = std::make_shared<Mesh>();
        mesh->loadMesh(filePath);
        mMeshes.emplace(filePath, mesh);
    }
    return mesh;
}

std::unique_ptr<IMeshLoader> AssetsManager::createMeshLoader(const std::string & filePath, std::vector<MeshVertices>& vertices) {
    std::unique_ptr<IMeshLoader> mesh = nullptr;

    //拡張子によって処理を分ける
    auto ext = FileUtil::getFileExtension(filePath);
    if (ext == ".obj") {
        mesh = std::make_unique<OBJ>();
    } else if (ext == ".fbx") {
        mesh = std::make_unique<FBX>();
    } else {
        Debug::windowMessage(filePath + ": 対応していない拡張子です");
    }

    World::instance().directory().setModelDirectory(filePath);
    auto fileName = FileUtil::getFileNameFromDirectry(filePath);
    mesh->perse(fileName, vertices);
    return mesh;
}

void AssetsManager::setDataDirectory(const std::string& filePath) const {
    const auto& dir = World::instance().directory();
    dir.setDataDirectory(filePath);
}
