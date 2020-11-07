#include "InputElementManager.h"
#include "../SystemInclude.h"
#include "../../DebugLayer/Debug.h"

InputElementManager::InputElementManager() {
    mInputLayouts.emplace("Texture.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Mesh.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("MeshTexture.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("NormalMap.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TANGENT", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("SkinMesh.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, VertexType::VERTEX_TYPE_FLOAT4, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "BLENDINDICES", 0, VertexType::VERTEX_TYPE_USHORT4, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("GBuffer.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Deferred.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Point3D.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Line2D.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Line3D.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("PointLight.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("SimpleMesh.hlsl", InputLayout{
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    });
}

InputElementManager::~InputElementManager() = default;

const std::vector<InputElementDesc>& InputElementManager::createInputLayout(const std::string& shaderName) const {
    auto itr = mInputLayouts.find(shaderName);
    //マップの中に存在している
    if (itr != mInputLayouts.end()) {
        return itr->second;
    } else {
        Debug::windowMessage(shaderName + "のInputLayoutが設定されていません");
    }
}
