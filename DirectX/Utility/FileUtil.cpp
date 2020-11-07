#include "FileUtil.h"
#include "StringUtil.h"
#include "../System/SystemInclude.h"
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

    return filePath.substr(0, pos + 1);
}

std::string FileUtil::getAssetsFromAbsolutePath(const std::string& absolutePath) {
    const std::string ASSETS = "Assets\\";
    auto pos = absolutePath.find(ASSETS);
    if (pos == std::string::npos) {
        return "";
    }

    return absolutePath.substr(pos);
}

bool FileUtil::openFileDialog(std::string& outFilePath, std::string& outFileName) {
    //今のディレクトパスを取得しとく
    char currentPath[256];
    GetCurrentDirectoryA(_countof(currentPath), currentPath);

    wchar_t path[MAX_PATH];
    wchar_t name[MAX_PATH];
    //文字列にヌル文字を代入しておく
    memset(path, '\0', sizeof(path));
    memset(name, '\0', sizeof(name));

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = path; //選択されたファイルパスを受け取る
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFileTitle = name; //選択されたファイル名
    ofn.nMaxFileTitle = MAX_PATH;

    //ofn.lpstrFilter = L"obj(*.obj)\0*.obj\0fbx(*.fbx)\0*.fbx\0all(*.*)\0*.*\0\0"; //フィルタ
    //ofn.lpstrTitle = L"Title"; //ダイアログボックスのタイトル

    ofn.Flags = OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&ofn) == FALSE) {
        return false;
    }

    //ファイル指定時に勝手にディレクトリがずれるから直す
    SetCurrentDirectoryA(currentPath);

    outFilePath = StringUtil::wcharToString(path);
    outFileName = StringUtil::wcharToString(name);

    return true;
}
