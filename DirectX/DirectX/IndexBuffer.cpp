#include "IndexBuffer.h"
#include "DirectX.h"

IndexBuffer::IndexBuffer(const BufferDesc& desc, const SubResourceDesc& data) :
    Buffer(desc, data) {
}

IndexBuffer::~IndexBuffer() = default;

void IndexBuffer::setIndexBuffer(Format format, unsigned offset) {
    DirectX::instance().deviceContext()->IASetIndexBuffer(mBuffer, toFormat(format), offset);
}
