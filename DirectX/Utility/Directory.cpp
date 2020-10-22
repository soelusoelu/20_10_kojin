#include "Directory.h"
#include "FileUtil.h"
#include "../System/SystemInclude.h"
#include <cassert>

Directory::Directory() :
    mRootPath("") {
    assert(!mInstantiated);
    mInstantiated = true;

    char dir[1024];
    GetCurrentDirectoryA(sizeof(dir), dir);
    mRootPath = dir;
}

Directory::~Directory() {
    mInstantiated = false;
}

void Directory::setRootDirectory() const {
    setCurrentDirectory(mRootPath);
}

void Directory::setAssetsDirectory() const {
    auto temp = mRootPath + "\\Assets/";
    setCurrentDirectory(temp);
}

void Directory::setShaderDirectory() const {
    auto temp = mRootPath + "\\Shader/";
    setCurrentDirectory(temp);
}

void Directory::setTextureDirectory(const std::string& filePath) const {
    auto temp = mRootPath + "\\Assets/Texture/";
    temp += FileUtil::getDirectryFromFilePath(filePath);

    setCurrentDirectory(temp);
}

void Directory::setSoundDirectory(const std::string& filePath) const {
    auto temp = mRootPath + "\\Assets/Sound/";
    temp += FileUtil::getDirectryFromFilePath(filePath);

    setCurrentDirectory(temp);
}

void Directory::setDataDirectory(const std::string& filePath) const {
    auto temp = mRootPath + "\\Assets/Data/";
    temp += FileUtil::getDirectryFromFilePath(filePath);

    setCurrentDirectory(temp);
}

void Directory::setModelDirectory(const std::string& filePath) const {
    auto temp = mRootPath + "\\Assets/Model/";
    temp += FileUtil::getDirectryFromFilePath(filePath);

    setCurrentDirectory(temp);
}

void Directory::setCurrentDirectory(const std::string& path) const {
    SetCurrentDirectoryA(path.c_str());
}
