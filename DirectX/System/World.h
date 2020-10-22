#pragma once

#include <memory>

class AssetsManager;
class Directory;

//グローバルアクセス専用クラス
class World {
private:
    World();
public:
    ~World();
    static World& instance();
    void finalize();
    AssetsManager& assetsManager() const;
    const Directory& directory() const;

private:
    World(const World&) = delete;
    World& operator=(const World&) = delete;

private:
    static inline World* mInstance = nullptr;
    std::unique_ptr<AssetsManager> mAssetsManager;
    std::unique_ptr<Directory> mDirectory;
};
