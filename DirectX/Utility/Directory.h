#pragma once

#include <string>

class Directory {
public:
    Directory();
    ~Directory();
    void setRootDirectory() const;
    void setAssetsDirectory() const;
    void setShaderDirectory() const;
    void setTextureDirectory(const std::string& filePath) const;
    void setSoundDirectory(const std::string& filePath) const;
    void setDataDirectory(const std::string& filePath) const;
    void setModelDirectory(const std::string& filePath) const;

private:
    Directory(const Directory&) = delete;
    Directory& operator=(const Directory&) = delete;

    void setCurrentDirectory(const std::string& path) const;

private:
    std::string mRootPath;

    static inline bool mInstantiated = false;
};
