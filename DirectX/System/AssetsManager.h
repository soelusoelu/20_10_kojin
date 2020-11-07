#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Mesh;
class TextureFromFile;

class AssetsManager {
private:
    //シングルトンだからprivate
    AssetsManager();
public:
    ~AssetsManager();
    //アセット管理クラスのインスタンスを返す
    static AssetsManager& instance();
    //終了処理
    void finalize();

    //テクスチャを読み込む
    void loadTexture(const std::string& fileName, const std::string& directoryPath = "Assets\\Texture\\");
    //ファイルパスからテクスチャを取得する
    std::shared_ptr<TextureFromFile> createTexture(const std::string& fileName, const std::string& directoryPath = "Assets\\Texture\\");

    //メッシュを読み込む
    void loadMesh(const std::string& fileName, const std::string& directoryPath = "Assets\\Model\\");
    //ファイルパスからメッシュを取得する
    std::shared_ptr<Mesh> createMesh(const std::string& fileName, const std::string& directoryPath = "Assets\\Model\\");

private:
    AssetsManager(const AssetsManager&) = delete;
    AssetsManager& operator=(const AssetsManager&) = delete;

    //読み込み済みのテクスチャか
    bool loadedTexture(const std::string& filePath) const;
    //読み込み済みのメッシュか
    bool loadedMesh(const std::string& filePath) const;

private:
    static inline AssetsManager* mInstance = nullptr;

    std::unordered_map<std::string, std::shared_ptr<TextureFromFile>> mTextures;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> mMeshes;
};
