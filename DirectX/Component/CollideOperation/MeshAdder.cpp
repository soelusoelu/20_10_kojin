#include "MeshAdder.h"
#include "../Mesh/MeshComponent.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Utility/FileUtil.h"

MeshAdder::MeshAdder(GameObject& gameObject)
    : Component(gameObject) {
}

MeshAdder::~MeshAdder() = default;

std::shared_ptr<MeshComponent> MeshAdder::addMesh(GameObject& gameObject) const {
    std::string filePath, fileName;
    if (!selectMeshFile(filePath, fileName)) {
        return nullptr;
    }

    return addMeshComponent(gameObject, filePath, fileName);
}

std::shared_ptr<MeshComponent> MeshAdder::addMeshCreateGameObject(const std::string& tag) const {
    std::string filePath, fileName;
    if (!selectMeshFile(filePath, fileName)) {
        return nullptr;
    }

    //ファイル名から拡張子を取り出す
    const auto& ext = FileUtil::getFileExtension(fileName);
    //ファイル名から拡張子を抜いた部分をゲームオブジェクトの名前とする
    auto name = fileName.substr(0, fileName.length() - ext.length());
    //すでにあるメッシュが選択された場合に名前被りを避けるため
    gameObject().getGameObjectManager().setNameNumber(name);
    //ゲームオブジェクト生成
    auto newGameObject = GameObject::create(name, tag);

    return addMeshComponent(*newGameObject, filePath, fileName);
}

std::shared_ptr<MeshComponent> MeshAdder::addMeshComponent(GameObject& gameObject, const std::string& filePath, const std::string& fileName) const {
    //絶対パスからアセットディレクトリ部分を抜き出す
    auto assetsDir = FileUtil::getAssetsFromAbsolutePath(filePath);
    //抜き出したアセットディレクトリからファイル名を抜いたディレクトリパスを取得する
    auto directoryPath = FileUtil::getDirectryFromFilePath(assetsDir);
    //ゲームオブジェクトにメッシュをアタッチする
    auto newMesh = Component::addComponent<MeshComponent>(gameObject, "MeshComponent");
    //メッシュとシェーダーを生成する
    newMesh->createMesh(fileName, directoryPath);
    newMesh->setDefaultShader();

    return newMesh;
}

bool MeshAdder::selectMeshFile(std::string& filePath, std::string& fileName) const {
    if (!FileUtil::openFileDialog(filePath, fileName)) {
        Debug::logError("Failed open dialog.");
        return false;
    }

    return true;
}
