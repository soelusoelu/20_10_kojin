#include "Mesh.h"
#include "OBJ.h"
#include "FBX/FBX.h"
#include "../DebugLayer/Debug.h"
#include "../DirectX/DirectXInclude.h"
#include "../System/Texture/TextureFromMemory.h"
#include "../Utility/FileUtil.h"
#include <cassert>

Mesh::Mesh()
    : mMesh(nullptr)
{
}

Mesh::~Mesh() = default;

const Material& Mesh::getMaterial(unsigned index) const {
    assert(index < mMaterials.size());
    return mMaterials[index];
}

unsigned Mesh::getMeshCount() const {
    return mMeshesVertices.size();
}

const MeshVertices& Mesh::getMeshVertices(unsigned index) const {
    assert(index < mMeshesVertices.size());
    return mMeshesVertices[index];
}

const Indices& Mesh::getMeshIndices(unsigned index) const {
    assert(index < mMeshesIndices.size());
    return mMeshesIndices[index];
}

unsigned Mesh::getPolygonCount(unsigned index) const {
    return getMeshIndices(index).size() / 3;
}

Triangle Mesh::getPolygon(unsigned meshIndex, unsigned polygonIndex) const {
    const auto& meshVertices = getMeshVertices(meshIndex);
    const auto& meshIndices = getMeshIndices(meshIndex);

    Triangle polygon{};
    polygon.p0 = meshVertices[meshIndices[polygonIndex * 3]].pos;
    polygon.p1 = meshVertices[meshIndices[polygonIndex * 3 + 1]].pos;
    polygon.p2 = meshVertices[meshIndices[polygonIndex * 3 + 2]].pos;

    return polygon;
}

Triangle Mesh::getPolygon(unsigned meshIndex, unsigned polygonIndex, const Matrix4& world) const {
    auto polygon = getPolygon(meshIndex, polygonIndex);

    polygon.p0 = Vector3::transform(polygon.p0, world);
    polygon.p1 = Vector3::transform(polygon.p1, world);
    polygon.p2 = Vector3::transform(polygon.p2, world);

    return polygon;
}

const Motion& Mesh::getMotion(unsigned index) const {
    assert(index < mMotions.size());
    return mMotions[index];
}

unsigned Mesh::getMotionCount() const {
    return mMotions.size();
}

void Mesh::setMotionName(const std::string& name, unsigned index) {
    assert(index < mMotions.size());
    mMotions[index].name = name;
}

const Bone& Mesh::getBone(unsigned index) const {
    return mBones[index];
}

unsigned Mesh::getBoneCount() const {
    return mBones.size();
}

void Mesh::draw(unsigned meshIndex) const {
    //バーテックスバッファーをセット
    mVertexBuffers[meshIndex]->setVertexBuffer();
    //インデックスバッファーをセット
    mIndexBuffers[meshIndex]->setIndexBuffer();

    //プリミティブをレンダリング
    MyDirectX::DirectX::instance().drawIndexed(mMeshesIndices[meshIndex].size());
}

void Mesh::loadMesh(const std::string& filePath) {
    //すでに生成済みなら終了する
    if (mMesh) {
        return;
    }

    initialize(filePath);
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

    //テクスチャがないマテリアルは白テクスチャを代替する
    for (auto&& mat : mMaterials) {
        if (!mat.texture) {
            mat.texture = std::make_shared<TextureFromMemory>(1, 1);
        }
    }
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
