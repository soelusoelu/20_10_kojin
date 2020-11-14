#include "OBJ.h"
#include "../DebugLayer/Debug.h"
#include "../System/AssetsManager.h"
#include "../System/Texture/TextureFromFile.h"
#include "../Utility/FileUtil.h"
#include <sstream>

OBJ::OBJ() = default;

OBJ::~OBJ() = default;

void OBJ::parse(
    const std::string& filePath,
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Indices>& meshesIndices,
    std::vector<Material>& materials,
    std::vector<Motion> & motions,
    std::vector<Bone> & bones
) {
    //OBJファイルを開いて内容を読み込む
    std::ifstream ifs(filePath);
    if (ifs.fail()) {
        Debug::windowMessage(filePath + ": ファイルが存在しません");
        return;
    }

    //ディレクトリパスを抜き出しておく
    auto directoryPath = FileUtil::getDirectryFromFilePath(filePath);

    //現状1つ
    meshesVertices.resize(1);
    meshesIndices.resize(1);
    materials.resize(1);

    //解析開始
    std::string line;
    while (!ifs.eof()) {
        //1行ずつ読み込む
        std::getline(ifs, line);

        //読み込みに不要な行ならスキップ
        if (isSkip(line)) {
            continue;
        }

        //1行分の文字列をストリームに変換して解析しやすくする
        std::istringstream lineStream(line);

        //半角スペース区切りで行の先頭文字列を取得
        std::string key;
        std::getline(lineStream, key, ' ');

        if (key == "v") { //先頭文字列がvなら頂点座標
            loadPosition(lineStream);
        } else if (key == "vt") { //先頭文字列がvtならuv座標
            loadUV(lineStream);
        } else if (key == "vn") { //先頭文字列がvnなら法線
            loadNormal(lineStream);
        } else if (key == "f") { //先頭文字列がfならポリゴン
            loadFace(meshesVertices.back(), meshesIndices.back(), lineStream);
        } else if (key == "mtllib") {
            loadMaterial(materials.back(), lineStream, directoryPath);
        }
    }
}

void OBJ::loadPosition(std::istringstream& iss) {
    //座標読み込み
    Vector3 pos;
    iss >> pos.x;
    iss >> pos.y;
    iss >> pos.z;
    //座標データに追加
    mPositions.emplace_back(pos);
}

void OBJ::loadNormal(std::istringstream& iss) {
    //xyz成分を読み込む
    Vector3 normal;
    iss >> normal.x;
    iss >> normal.y;
    iss >> normal.z;
    //法線ベクトルデータに追加
    mNormals.emplace_back(normal);
}

void OBJ::loadUV(std::istringstream& iss) {
    //UV成分を読み込む
    Vector2 uv;
    iss >> uv.x;
    iss >> uv.y;
    //V方向反転
    uv.y = 1.f - uv.y;
    //テクスチャ座標データに追加
    mUVs.emplace_back(uv);
}

void OBJ::loadFace(
    MeshVertices& meshVertices,
    Indices& indices,
    std::istringstream& iss
) {
    //半角スペース区切りで行の続きを読み込む
    std::string indexString;
    while (std::getline(iss, indexString, ' ')) {
        //頂点データを追加していく
        MeshVertex vertex{};
        std::istringstream indexStream(indexString);
        unsigned short indexPos, indexNormal, indexUV;

        indexStream >> indexPos;
        vertex.pos = mPositions[indexPos - 1];

        //1文字分読み飛ばす
        indexStream.seekg(1, std::ios_base::cur);

        indexStream >> indexUV;
        //テクスチャがない場合の対応
        if (indexUV == 0) {
            indexStream.clear();
        } else {
            //テクスチャが有るなら追加
            vertex.uv = mUVs[indexUV - 1];
        }

        //1文字分読み飛ばす
        indexStream.seekg(1, std::ios_base::cur);

        indexStream >> indexNormal;
        //法線が有るなら追加
        if (indexNormal > 0) {
            vertex.normal = mNormals[indexNormal - 1];
        }

        //頂点データを登録
        meshVertices.emplace_back(vertex);

        //頂点インデックスに追加
        indices.emplace_back(indices.size());
    }
}

void OBJ::loadMaterial(
    Material& material,
    std::istringstream& iss,
    const std::string& directoryPath
) {
    //マテリアルのファイル名読み込み
    std::string fileName;
    iss >> fileName;

    //取得したファイル名とディレクトリパスを結合する
    auto filePath = directoryPath + fileName;

    //マテリアルファイルを開く
    std::ifstream ifs(filePath);
    if (ifs.fail()) {
        Debug::windowMessage(filePath + ": ファイルが存在しません");
        return;
    }

    //解析開始
    std::string line;
    while (!ifs.eof()) {
        //1行ずつ読み込む
        std::getline(ifs, line);

        //読み込みに不要な行ならスキップ
        if (isSkip(line)) {
            continue;
        }

        //1行分の文字列をストリームに変換
        std::istringstream lineStream(line);

        //半角スペース区切りで行の先頭文字列を取得
        std::string key;
        std::getline(lineStream, key, ' ');

        //先頭のタブ文字は無視する
        if (key[0] == '\t') {
            //先頭の文字を削除
            key.erase(key.begin());
        }

        //マテリアルの各属性を読み込む
        if (key == "newmtl") {
            loadMaterialName(material, lineStream);
        } else if (key == "Ka") {
            loadAmbient(material, lineStream);
        } else if (key == "Kd") {
            loadDiffuse(material, lineStream);
        } else if (key == "Ks") {
            loadSpecular(material, lineStream);
        } else if (key == "map_Kd") {
            loadTexture(material, lineStream, directoryPath);
        }
    }
}

void OBJ::loadMaterialName(Material& material, std::istringstream& iss) {
    iss >> material.materialName;
}

void OBJ::loadAmbient(Material& material, std::istringstream& iss) {
    iss >> material.ambient.x;
    iss >> material.ambient.y;
    iss >> material.ambient.z;
}

void OBJ::loadDiffuse(Material& material, std::istringstream& iss) {
    iss >> material.diffuse.x;
    iss >> material.diffuse.y;
    iss >> material.diffuse.z;
}

void OBJ::loadSpecular(Material& material, std::istringstream& iss) {
    iss >> material.specular.x;
    iss >> material.specular.y;
    iss >> material.specular.z;
}

void OBJ::loadTexture(Material& material, std::istringstream& iss, const std::string& directoryPath) {
    //テクスチャ名読み込み
    std::string textureName;
    iss >> textureName;
    //テクスチャ読み込み
    material.texture = AssetsManager::instance().createTexture(textureName, directoryPath);
}

bool OBJ::isSkip(const std::string& line) {
    //空白かコメントだったらスキップ
    return (line.empty() || line[0] == '#');
}
