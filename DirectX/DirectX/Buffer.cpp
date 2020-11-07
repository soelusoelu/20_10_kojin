#include "Buffer.h"
#include "DirectX.h"
#include "Usage.h"
#include "../System/GlobalFunction.h"

Buffer::Buffer(const BufferDesc& desc) :
    mDesc(desc),
    mBuffer(nullptr) {
    //バッファの作成
    MyDirectX::DirectX::instance().device()->CreateBuffer(&toBufferDesc(desc), nullptr, &mBuffer);
}

Buffer::Buffer(const BufferDesc& desc, const SubResourceDesc& data) :
    mDesc(desc),
    mBuffer(nullptr) {
    //バッファの作成
    MyDirectX::DirectX::instance().device()->CreateBuffer(&toBufferDesc(desc), &toSubResource(data), &mBuffer);
}

Buffer::~Buffer() {
    safeRelease(mBuffer);
}

const BufferDesc& Buffer::desc() const {
    return mDesc;
}

ID3D11Buffer* Buffer::buffer() const {
    return mBuffer;
}

D3D11_BUFFER_DESC Buffer::toBufferDesc(const BufferDesc& desc) const {
    D3D11_BUFFER_DESC bd;
    bd.ByteWidth = desc.size;
    bd.Usage = toUsage(desc.usage);
    bd.BindFlags = desc.type;
    bd.CPUAccessFlags = desc.cpuAccessFlags;
    bd.MiscFlags = desc.miscFlags;
    bd.StructureByteStride = desc.structureByteStride;

    return bd;
}
