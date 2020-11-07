#include "AssetsManager.h"
#include "../Mesh/Mesh.h"
#include "../System/GlobalFunction.h"
#include "../System/Texture/TextureFromFile.h"

AssetsManager::AssetsManager() = default;

AssetsManager::~AssetsManager() = default;

AssetsManager& AssetsManager::instance() {
    if (!mInstance) {
        mInstance = new AssetsManager();
    }
    return *mInstance;
}

void AssetsManager::finalize() {
    safeDelete(mInstance);
}

void AssetsManager::loadTexture(const std::string& fileName, const std::string& directoryPath) {
    //ディレクトパスとファイル名を結合する
    auto filePath = directoryPath + fileName;

    //読み込み済みなら何もしない
    if (loadedTexture(filePath)) {
        return;
    }

    //テクスチャを生成し格納
    auto texture = std::make_shared<TextureFromFile>(filePath);
    mTextures.emplace(filePath, texture);
}

std::shared_ptr<TextureFromFile> AssetsManager::createTexture(const std::string & fileName, const std::string& directoryPath) {
    //テクスチャを読み込む
    loadTexture(fileName, directoryPath);

    //読み込んだテクスチャを返す
    return mTextures[directoryPath + fileName];
}

void AssetsManager::loadMesh(const std::string& fileName, const std::string& directoryPath) {
    //ディレクトパスとファイル名を結合する
    auto filePath = directoryPath + fileName;

    //読み込み済みなら何もしない
    if (loadedMesh(filePath)) {
        return;
    }

    //メッシュを生成し格納
    auto mesh = std::make_shared<Mesh>();
    mesh->loadMesh(filePath);
    mMeshes.emplace(filePath, mesh);
}

std::shared_ptr<Mesh> AssetsManager::createMesh(const std::string& fileName, const std::string& directoryPath) {
    //メッシュを読み込む
    loadMesh(fileName, directoryPath);

    //読み込んだメッシュを返す
    return mMeshes[directoryPath + fileName];
}

bool AssetsManager::loadedTexture(const std::string& filePath) const {
    auto itr = mTextures.find(filePath);
    return (itr != mTextures.end());
}

bool AssetsManager::loadedMesh(const std::string& filePath) const {
    auto itr = mMeshes.find(filePath);
    return (itr != mMeshes.end());
}
