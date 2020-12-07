#include "Buffer.h"
#include "DirectX.h"
#include "Usage.h"

Buffer::Buffer(const BufferDesc& desc, const SubResourceDesc* data) :
    mDesc(desc),
    mBuffer(nullptr) {
    //バッファの作成
    const auto& temp = toBufferDesc(desc);
    if (data) {
        const auto& sub = toSubResource(*data);
        MyDirectX::DirectX::instance().device()->CreateBuffer(&temp, &sub, &mBuffer);
    } else {
        MyDirectX::DirectX::instance().device()->CreateBuffer(&temp, nullptr, &mBuffer);
    }
}

Buffer::~Buffer() = default;

const BufferDesc& Buffer::desc() const {
    return mDesc;
}

ID3D11Buffer* Buffer::buffer() const {
    return mBuffer.Get();
}

ID3D11Buffer* const* Buffer::bufferAddres() const {
    return mBuffer.GetAddressOf();
}

D3D11_BUFFER_DESC Buffer::toBufferDesc(const BufferDesc& desc) const {
    D3D11_BUFFER_DESC bd{};
    bd.ByteWidth = desc.size;
    bd.Usage = toUsage(desc.usage);
    bd.BindFlags = desc.type;
    bd.CPUAccessFlags = desc.cpuAccessFlags;
    bd.MiscFlags = desc.miscFlags;
    bd.StructureByteStride = desc.structureByteStride;

    return bd;
}
