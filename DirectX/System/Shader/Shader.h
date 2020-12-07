#pragma once

#include "../SystemInclude.h"
#include "../../DirectX/InputElementDesc.h"
#include <memory>
#include <string>
#include <vector>

class Buffer;
class InputElement;
class ConstantBufferManager;
class InputElementManager;

class Shader {
    using BufferPtr = std::unique_ptr<Buffer>;
    using BufferPtrArray = std::vector<BufferPtr>;

public:
    Shader(const std::string& fileName, const std::string& directoryPath);
    ~Shader();

    //プログラムの終わりの終了処理
    static void finalize();

    //描画に必要な要素をすべて登録する
    void setShaderInfo() const;
    //シェーダーにデータを転送する
    void transferData(const void* data, unsigned size, unsigned constantBufferIndex = 0) const;

    //個別に登録したいなら↓
    //自身をシェーダーとして登録
    void setVSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0) const;
    void setPSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0) const;
    //使用するコンスタントバッファを登録
    void setVSConstantBuffers(unsigned index = 0, unsigned numBuffers = 1) const;
    void setPSConstantBuffers(unsigned index = 0, unsigned numBuffers = 1) const;
    //自身を登録
    void setInputLayout() const;

    //シェーダー名を取得する
    const std::string& getShaderName() const;

private:
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    //シェーダの生成
    void createVertexShader(const std::string& fileName);
    void createPixelShader(const std::string& fileName);
    //シェーダーをコンパイルする
    bool compileShader(Microsoft::WRL::ComPtr<ID3DBlob>* out, const std::string& fileName, const std::string& entryPoint, const std::string& target);
    //インプットレイアウトの生成
    void createInputLayout(const std::vector<InputElementDesc>& layout);
    //シェーダーに値を渡すための開始・終了処理
    bool map(D3D11_MAPPED_SUBRESOURCE* mapRes, unsigned index = 0, unsigned sub = 0, D3D11_MAP type = D3D11_MAP_WRITE_DISCARD, unsigned flag = 0) const;
    void unmap(unsigned index = 0, unsigned sub = 0) const;

private:
    std::string mShaderName;
    Microsoft::WRL::ComPtr<ID3DBlob> mVSBlob;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
    BufferPtrArray mConstantBuffers;
    std::unique_ptr<InputElement> mVertexLayout;

    static inline ConstantBufferManager* constantBufferManager = nullptr;
    static inline InputElementManager* inputElementManager = nullptr;
};
