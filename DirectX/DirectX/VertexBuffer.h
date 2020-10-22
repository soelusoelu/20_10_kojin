#pragma once

#include "Buffer.h"
#include "BufferDesc.h"
#include "SubResourceDesc.h"

class VertexBuffer : public Buffer {
public:
    VertexBuffer(const BufferDesc& desc, const SubResourceDesc& data);
    ~VertexBuffer();
    void setVertexBuffer(unsigned start = 0, unsigned numStream = 1, unsigned offset = 0);
};
