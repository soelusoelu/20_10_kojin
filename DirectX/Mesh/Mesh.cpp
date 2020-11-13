#include "Mesh.h"
#include "OBJ.h"
#include "FBX/FBX.h"
#include "../DebugLayer/Debug.h"
#include "../DirectX/DirectXInclude.h"
#include "../Utility/FileUtil.h"
#include <cassert>

Mesh::Mesh() :
    mMesh(nullptr) {
}

Mesh::~Mesh() = default;

const Material& Mesh::getMaterial(unsigned index) const {
    return mMaterials[index];
}

unsigned Mesh::getMeshCount() const {
    return mMeshesVertices.size();
}

const MeshVertices& Mesh::getMeshVertices(unsigned index) const {
    return mMeshesVertices[index];
}

const Motion& Mesh::getMotion(unsigned index) const {
    return mMotions[index];
}

unsigned Mesh::getMotionCount() const {
    return mMotions.size();
}

const Bone& Mesh::getBone(unsigned index) const {
    return mBones[index];
}

unsigned Mesh::getBoneCount() const {
    return mBones.size();
}

void Mesh::loadMesh(const std::string& filePath) {
    //すでに生成済みなら終了する
    if (mMesh) {
        return;
    }

    initialize(filePath);
}

void Mesh::draw(unsigned meshIndex) const {
    //バーテックスバッファーをセット
    mVertexBuffers[meshIndex]->setVertexBuffer();
    //インデックスバッファーをセット
    mIndexBuffers[meshIndex]->setIndexBuffer();

    //プリミティブをレンダリング
    MyDirectX::DirectX::instance().drawIndexed(mMeshesIndices[meshIndex].size());
}

void Mesh::initialize(const std::string& filePath) {
    //ファイルパスからメッシュを作成
    createMesh(filePath);

    //それぞれは同じサイズのはず
    assert(mMeshesVertices.size() == mMeshesIndices.size());
    assert(mMeshesVertices.size() == mMaterials.size());

    //メッシュの数だけバッファを作る
    for (size_t i = 0; i < mMeshesVertices.size(); i++) {
        createVertexBuffer(i);
        createIndexBuffer(i);
    }
}

void Mesh::createMesh(const std::string& filePath) {
    //拡張子によって処理を分ける
    auto ext = FileUtil::getFileExtension(filePath);
    if (ext == ".obj") {
        mMesh = std::make_unique<OBJ>();
    } else if (ext == ".fbx") {
        mMesh = std::make_unique<FBX>();
    } else {
        Debug::windowMessage(filePath + ": 対応していない拡張子です");
        return;
    }

    //メッシュを解析する
    mMesh->parse(filePath, mMeshesVertices, mMeshesIndices, mMaterials, mMotions, mBones);
}

void Mesh::createVertexBuffer(unsigned meshIndex) {
    const auto& vertices = mMeshesVertices[meshIndex];
    BufferDesc bd;
    bd.oneSize = sizeof(vertices[0]);
    bd.size = bd.oneSize * vertices.size();
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);
    SubResourceDesc sub;
    sub.data = vertices.data();

    mVertexBuffers.emplace_back(std::make_unique<VertexBuffer>(bd, sub));
}

void Mesh::createIndexBuffer(unsigned meshIndex) {
    const auto& indices = mMeshesIndices[meshIndex];
    BufferDesc bd;
    bd.size = sizeof(indices[0]) * indices.size();
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_INDEX);
    SubResourceDesc sub;
    sub.data = indices.data();

    mIndexBuffers.emplace_back(std::make_unique<IndexBuffer>(bd, sub));
}
