#pragma once

#include "../../DirectX/InputElementDesc.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class InputElementManager {
    using InputLayout = std::vector<InputElementDesc>;

public:
    InputElementManager();
    ~InputElementManager();
    const InputLayout& createInputLayout(const std::string& shaderName) const;

private:
    InputElementManager(const InputElementManager&) = delete;
    InputElementManager& operator=(const InputElementManager&) = delete;

private:
    std::unordered_map<std::string, InputLayout> mInputLayouts;
};
