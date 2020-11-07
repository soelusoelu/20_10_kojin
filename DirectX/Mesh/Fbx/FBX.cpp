#include "FBX.h"
#include "FbxBoneParser.h"
#include "FbxBoneWeightParser.h"
#include "FbxMaterialParser.h"
#include "FbxMeshParser.h"
#include "../Material.h"
#include "../../DebugLayer/Debug.h"
#include "../../Utility/FileUtil.h"

FBX::FBX()
    : mMeshParser(std::make_unique<FbxMeshParser>())
    , mMaterialParser(std::make_unique<FbxMaterialParser>())
    , mBoneParser(std::make_unique<FbxBoneParser>())
    , mBoneWeightParser(std::make_unique<FbxBoneWeightParser>())
{
}

FBX::~FBX() = default;

void FBX::parse(
    const std::string& filePath,
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Indices>& meshesIndices,
    std::vector<Material>& materials,
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
        bones,
        scene,
        filePath
    );

    //ボーンの読み込み
    mBoneParser->parse(meshesVertices, bones, scene);

    //マネージャー解放
    manager->Destroy();
}

void FBX::createMeshes(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Indices>& meshesIndices,
    std::vector<Material>& materials,
    std::vector<Bone>& bones,
    const FbxScene* fbxScene,
    const std::string& filePath
) {
    //FbxMeshの数を取得
    auto numMeshes = fbxScene->GetSrcObjectCount<FbxMesh>();

    //メッシュの数に合わせて拡張する
    meshesVertices.resize(numMeshes);
    meshesIndices.resize(numMeshes);
    materials.resize(numMeshes);

    //ファイルパスからディレクトリパスを抜き出す
    auto directoryPath = FileUtil::getDirectryFromFilePath(filePath);

    //FbxMeshの数だけメッシュを作成する
    for (size_t i = 0; i < numMeshes; ++i) {
        auto mesh = fbxScene->GetSrcObject<FbxMesh>(i);
        createMesh(
            meshesVertices[i],
            meshesIndices[i],
            materials[i],
            bones,
            mesh,
            directoryPath
        );
    }
}

void FBX::createMesh(
    MeshVertices& meshVertices,
    Indices& indices,
    Material& material,
    std::vector<Bone>& bones,
    FbxMesh* fbxMesh,
    const std::string& directoryPath
) {
    //メッシュの読み込み
    mMeshParser->parse(meshVertices, indices, fbxMesh);
    //マテリアルの読み込み
    mMaterialParser->parse(material, fbxMesh, directoryPath);
    //ボーンウェイトの読み込み
    mBoneWeightParser->parse(meshVertices, fbxMesh);
}
