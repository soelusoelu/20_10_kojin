#pragma once

#include "../Component.h"
#include <memory>
#include <string>

class MeshComponent;

//メッシュを追加するクラス
class MeshAdder : public Component {
public:
    MeshAdder(GameObject& gameObject);
    ~MeshAdder();

    //指定のゲームオブジェクトにメッシュコンポーネントをアタッチする
    std::shared_ptr<MeshComponent> addMesh(GameObject& gameObject) const;
    //ゲームオブジェクトを生成し、メッシュコンポーネントをアタッチする
    std::shared_ptr<MeshComponent> addMeshCreateGameObject(const std::string& tag = "None") const;

private:
    MeshAdder(const MeshAdder&) = delete;
    MeshAdder& operator=(const MeshAdder&) = delete;

    std::shared_ptr<MeshComponent> addMeshComponent(GameObject& gameObject, const std::string& filePath, const std::string& fileName) const;
    bool selectMeshFile(std::string& filePath, std::string& fileName) const;
};
