#include "ConstantBufferManager.h"
#include "ConstantBuffers.h"
#include "../../DebugLayer/Debug.h"
#include "../../DirectX/DirectXInclude.h"

ConstantBufferManager::ConstantBufferManager() {
    mConstantBuffers.emplace("Texture.hlsl", BuffersSize{ sizeof(TextureConstantBuffer) });
    mConstantBuffers.emplace("Texture3D.hlsl", BuffersSize{ sizeof(TextureConstantBuffer) });
    mConstantBuffers.emplace("Mesh.hlsl", BuffersSize{ sizeof(MeshCommonConstantBuffer), sizeof(MaterialConstantBuffer) });
    mConstantBuffers.emplace("NormalMap.hlsl", BuffersSize{ sizeof(MeshCommonConstantBuffer), sizeof(MaterialConstantBuffer) });
    mConstantBuffers.emplace("SkinMesh.hlsl", BuffersSize{ sizeof(MeshCommonConstantBuffer), sizeof(MaterialConstantBuffer), sizeof(SkinMeshConstantBuffer) });
    mConstantBuffers.emplace("GBuffer.hlsl", BuffersSize{ sizeof(MeshConstantBuffer), sizeof(MaterialConstantBuffer) });
    mConstantBuffers.emplace("Deferred.hlsl", BuffersSize{ sizeof(GBufferShaderConstantBuffer) });
    mConstantBuffers.emplace("Point3D.hlsl", BuffersSize{ sizeof(PointConstantBuffer) });
    mConstantBuffers.emplace("Line2D.hlsl", BuffersSize{ sizeof(LineConstantBuffer) });
    mConstantBuffers.emplace("Line3D.hlsl", BuffersSize{ sizeof(LineConstantBuffer) });
    mConstantBuffers.emplace("PointLight.hlsl", BuffersSize{ sizeof(PointLightConstantBuffer) });
    mConstantBuffers.emplace("SimpleMesh.hlsl", BuffersSize{ sizeof(SimpleMeshConstantBuffer) });
    mConstantBuffers.emplace("OutLine.hlsl", BuffersSize{ sizeof(OutLineConstantBuffer) });
    mConstantBuffers.emplace("SkinMeshOutLine.hlsl", BuffersSize{ sizeof(OutLineConstantBuffer), sizeof(SkinMeshConstantBuffer) });
    mConstantBuffers.emplace("ShadowDepthTextureCreater.hlsl", BuffersSize{ sizeof(SimpleMeshConstantBuffer) });
    mConstantBuffers.emplace("Shadow.hlsl", BuffersSize{ sizeof(MeshCommonConstantBuffer), sizeof(MaterialConstantBuffer), sizeof(ShadowConstantBuffer) });
    mConstantBuffers.emplace("SkinMeshShadow.hlsl", BuffersSize{ sizeof(MeshCommonConstantBuffer), sizeof(MaterialConstantBuffer), sizeof(ShadowConstantBuffer), sizeof(SkinMeshConstantBuffer) });
}

ConstantBufferManager::~ConstantBufferManager() = default;

std::vector<std::unique_ptr<Buffer>> ConstantBufferManager::createConstantBuffer(const std::string& shaderName) const {
    BufferPtrArray buffers;
    auto itr = mConstantBuffers.find(shaderName);
    //マップの中に存在している
    if (itr != mConstantBuffers.end()) {
        //バッファの数を取得
        auto size = itr->second.size();
        //バッファの数だけ拡張する
        buffers.resize(size);

        //バッファ共通設定
        BufferDesc cb;
        cb.usage = Usage::USAGE_DYNAMIC;
        cb.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_CONSTANT_BUFFER);
        cb.cpuAccessFlags = static_cast<unsigned>(BufferCPUAccessFlag::CPU_ACCESS_WRITE);

        //バッファの数だけ生成する
        for (size_t i = 0; i < size; i++) {
            cb.size = ((itr->second[i] + 0xff) & ~0xff);
            buffers[i] = std::make_unique<Buffer>(cb);
        }
    } else {
        Debug::windowMessage(shaderName + "のConstantBufferが設定されていません");
    }

    return buffers;
}
