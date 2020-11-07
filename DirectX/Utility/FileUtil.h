#pragma once

#include <string>

class FileUtil {
    FileUtil() = delete;
    ~FileUtil() = delete;

public:
    //パスからファイル拡張子を取得する
    static std::string getFileExtension(const std::string& filePath);
    //パスからファイル名を取得する
    static std::string getFileNameFromDirectry(const std::string& filePath);
    //パスからディレクトリ部分を取得する
    static std::string getDirectryFromFilePath(const std::string& filePath);
    //絶対パスからAssetsフォルダ部分を抜き出す
    static std::string getAssetsFromAbsolutePath(const std::string& absolutePath);
    //ファイルダイアログを開く
    static bool openFileDialog(std::string& outFilePath, std::string& outFileName);
};
