#include "FileUtil.h"
#include <algorithm>

std::string FileUtil::getFileExtension(const std::string& filePath) {
    std::string fileExtension;
    size_t pos = filePath.find_last_of('.');
    if (pos != std::string::npos) {
        fileExtension = filePath.substr(pos, filePath.length());

        std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
    }

    return fileExtension;
}

std::string FileUtil::getFileNameFromDirectry(const std::string& filePath) {
    auto pos = filePath.find_last_of('/');
    if (pos == std::string::npos) {
        pos = filePath.find_last_of('\\');
        if (pos == std::string::npos) {
            return filePath;
        }
    }
    return filePath.substr(pos + 1);
}

std::string FileUtil::getDirectryFromFilePath(const std::string& filePath) {
    auto pos = filePath.find_last_of('/');
    if (pos == std::string::npos) {
        pos = filePath.find_last_of('\\');
        if (pos == std::string::npos) {
            return "";
        }
    }
    return filePath.substr(0, pos);
}
