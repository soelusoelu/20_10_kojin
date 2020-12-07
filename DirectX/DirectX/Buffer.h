#pragma once

#include "BufferDesc.h"
#include "SubResourceDesc.h"
#include "../System/SystemInclude.h"

class Buffer {
public:
    Buffer(const BufferDesc& desc, const SubResourceDesc* data = nullptr);
    virtual ~Buffer();
    const BufferDesc& desc() const;
    ID3D11Buffer* buffer() const;
    ID3D11Buffer* const* bufferAddres() const;

private:
    D3D11_BUFFER_DESC toBufferDesc(const BufferDesc& desc) const;

    //コピー禁止
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

protected:
    BufferDesc mDesc;
    Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
};

