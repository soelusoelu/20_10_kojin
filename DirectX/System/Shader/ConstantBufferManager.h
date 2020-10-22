#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Buffer;

class ConstantBufferManager {
    using BufferPtr = std::unique_ptr<Buffer>;
    using BufferPtrArray = std::vector<BufferPtr>;
    using BuffersSize = std::vector<unsigned>;

public:
    ConstantBufferManager();
    ~ConstantBufferManager();
    BufferPtrArray createConstantBuffer(const std::string& shaderName) const;

private:
    ConstantBufferManager(const ConstantBufferManager&) = delete;
    ConstantBufferManager& operator=(const ConstantBufferManager&) = delete;

private:
    std::unordered_map<std::string, BuffersSize> mConstantBuffers;
};
