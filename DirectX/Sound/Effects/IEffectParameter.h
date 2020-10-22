#pragma once

//エフェクトパラメータインターフェース
class IEffectParameter {
public:
    virtual ~IEffectParameter() = default;
    virtual void setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize) = 0;
    virtual void getEffectParameters(int effectID, void* parameters, unsigned parametersByteSize) const = 0;
};
