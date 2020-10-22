#include "World.h"
#include "../Device/AssetsManager.h"
#include "../System/GlobalFunction.h"
#include "../Utility/Directory.h"

World::World() :
    mAssetsManager(std::make_unique<AssetsManager>()),
    mDirectory(std::make_unique<Directory>()) {
}

World::~World() = default;

World& World::instance() {
    if (!mInstance) {
        mInstance = new World();
    }
    return *mInstance;
}

void World::finalize() {
    safeDelete(mInstance);
}

AssetsManager& World::assetsManager() const {
    return *mAssetsManager;
}

const Directory& World::directory() const {
    return *mDirectory;
}
