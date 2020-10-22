#include "OBJ.h"
#include "../DebugLayer/Debug.h"
#include "../Device/AssetsManager.h"
#include "../System/World.h"
#include <sstream>

OBJ::OBJ() = default;

OBJ::~OBJ() = default;

void OBJ::perse(const std::string& fileName, std::vector<MeshVertices>& meshes) {
    //OBJファイルを開いて内容を読み込む
    std::ifstream ifs(fileName);
    if (ifs.fail()) {
        Debug::windowMessage(fileName + ": ファイルが存在しません");
        return;
    }

    //現状1つ
    meshes.resize(1);

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
            loadFace(lineStream, meshes.back());
        } else if (key == "mtllib") {
            loadMaterial(lineStream);
        }
    }
}

const std::vector<unsigned short>& OBJ::getIndices(unsigned meshIndex) const {
    return mIndices;
}

const Material& OBJ::getMaterial(unsigned index) const {
    return mMaterials[index];
}

unsigned OBJ::getMeshCount() const {
    return 1;
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

void OBJ::loadFace(std::istringstream& iss, MeshVertices& meshVertices) {
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
        mIndices.emplace_back(mIndices.size());
    }
}

void OBJ::loadMaterial(std::istringstream& iss) {
    //マテリアルのファイル名読み込み
    std::string fileName;
    iss >> fileName;

    //マテリアルファイルを開く
    std::ifstream ifs(fileName);
    if (ifs.fail()) {
        Debug::windowMessage(fileName + ": ファイルが存在しません");
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
            loadMaterialName(lineStream);
        } else if (key == "Ka") {
            loadAmbient(lineStream);
        } else if (key == "Kd") {
            loadDiffuse(lineStream);
        } else if (key == "Ks") {
            loadSpecular(lineStream);
        } else if (key == "map_Kd") {
            loadTexture(lineStream);
        }
    }
}

void OBJ::loadMaterialName(std::istringstream& iss) {
    mMaterials.resize(mMaterials.size() + 1);
    iss >> mMaterials.back().materialName;
}

void OBJ::loadAmbient(std::istringstream& iss) {
    auto& mat = mMaterials.back();
    iss >> mat.ambient.x;
    iss >> mat.ambient.y;
    iss >> mat.ambient.z;
}

void OBJ::loadDiffuse(std::istringstream& iss) {
    auto& mat = mMaterials.back();
    iss >> mat.diffuse.x;
    iss >> mat.diffuse.y;
    iss >> mat.diffuse.z;
}

void OBJ::loadSpecular(std::istringstream& iss) {
    auto& mat = mMaterials.back();
    iss >> mat.specular.x;
    iss >> mat.specular.y;
    iss >> mat.specular.z;
}

void OBJ::loadTexture(std::istringstream& iss) {
    auto& mat = mMaterials.back();
    //テクスチャ名読み込み
    std::string textureName;
    iss >> textureName;
    //テクスチャ読み込み
    mat.texture = World::instance().assetsManager().createTextureFromModel(textureName);
}

bool OBJ::isSkip(const std::string& line) {
    //空白かコメントだったらスキップ
    return (line.empty() || line[0] == '#');
}
