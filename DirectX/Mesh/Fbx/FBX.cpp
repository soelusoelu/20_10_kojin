#include "FBX.h"
#include "FbxMaterialParser.h"
#include "FbxMeshParser.h"
#include "FbxMotionParser.h"
#include "../Material.h"
#include "../../DebugLayer/Debug.h"
#include "../../Utility/FileUtil.h"

FBX::FBX()
    : mMeshParser(std::make_unique<FbxMeshParser>())
    , mMaterialParser(std::make_unique<FbxMaterialParser>())
    , mMotionParser(std::make_unique<FbxMotionParser>())
{
}

FBX::~FBX() = default;

void FBX::parse(
    const std::string& filePath,
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Indices>& meshesIndices,
    std::vector<Material>& materials,
    std::vector<Motion>& motions,
    std::vector<Bone>& bones
) {
    //マネージャーを生成
    auto manager = FbxManager::Create();

    //IOSettingを生成
    FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ioSettings);

    //Importerを生成
    FbxImporter* importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize(filePath.c_str(), -1, manager->GetIOSettings())) {
        Debug::windowMessage(filePath + ": ファイルは存在しません");
        return;
    }

    //SceneオブジェクトにFBXファイル内の情報を流し込む
    FbxScene* scene = FbxScene::Create(manager, "scene");
    importer->Import(scene);
    importer->Destroy(); //シーンを流し込んだらImporterは解放していい

    //四角ポリゴンを三角ポリゴンに変換
    FbxGeometryConverter geometryConverter(manager);
    geometryConverter.Triangulate(scene, true);

    //マテリアル単位でメッシュを分割する
    geometryConverter.SplitMeshesPerMaterial(scene, true);

    //全メッシュの作成
    createMeshes(
        meshesVertices,
        meshesIndices,
        materials,
        scene,
        filePath
    );

    //モーション読み込み
    mMotionParser->parse(meshesVertices, motions, bones, scene, mFbxMeshes);

    //マネージャー解放
    manager->Destroy();
}

void FBX::createMeshes(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Indices>& meshesIndices,
    std::vector<Material>& materials,
    const FbxScene* fbxScene,
    const std::string& filePath
) {
    //FbxMeshの数を取得
    auto numMeshes = fbxScene->GetSrcObjectCount<FbxMesh>();

    //メッシュの数に合わせて拡張する
    meshesVertices.resize(numMeshes);
    meshesIndices.resize(numMeshes);
    materials.resize(numMeshes);
    mFbxMeshes.resize(numMeshes);

    //ファイルパスからディレクトリパスを抜き出す
    auto directoryPath = FileUtil::getDirectryFromFilePath(filePath);

    //FbxMeshの数だけメッシュを作成する
    for (int i = 0; i < numMeshes; ++i) {
        auto mesh = fbxScene->GetSrcObject<FbxMesh>(i);
        mFbxMeshes[i] = mesh;

        createMesh(
            meshesVertices[i],
            meshesIndices[i],
            materials[i],
            mesh,
            directoryPath
        );
    }
}

void FBX::createMesh(
    MeshVertices& meshVertices,
    Indices& indices,
    Material& material,
    FbxMesh* fbxMesh,
    const std::string& directoryPath
) {
    //メッシュの読み込み
    mMeshParser->parse(meshVertices, indices, fbxMesh);
    //マテリアルの読み込み
    mMaterialParser->parse(material, fbxMesh, directoryPath);
}
