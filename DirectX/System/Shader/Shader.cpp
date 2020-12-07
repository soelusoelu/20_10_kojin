#include "Shader.h"
#include "ConstantBufferManager.h"
#include "InputElementManager.h"
#include "../GlobalFunction.h"
#include "../../DebugLayer/Debug.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Utility/FileUtil.h"
#include "../../Utility/StringUtil.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

Shader::Shader(const std::string& fileName, const std::string& directoryPath) :
    mShaderName(fileName),
    mVSBlob(nullptr),
    mVertexShader(nullptr),
    mPixelShader(nullptr),
    mVertexLayout(nullptr) {
    if (!constantBufferManager || !inputElementManager) {
        constantBufferManager = new ConstantBufferManager();
        inputElementManager = new InputElementManager();
    }

    //ディレクトパスとファイル名を結合する
    auto filePath = directoryPath + fileName;

    createVertexShader(filePath);
    createPixelShader(filePath);
    mConstantBuffers = constantBufferManager->createConstantBuffer(fileName);
    createInputLayout(inputElementManager->createInputLayout(fileName));
}

Shader::~Shader() = default;

void Shader::finalize() {
    safeDelete(constantBufferManager);
    safeDelete(inputElementManager);
}

void Shader::setShaderInfo() const {
    setVSShader();
    setPSShader();
    setInputLayout();
}

void Shader::transferData(const void* data, unsigned size, unsigned constantBufferIndex) const {
    //コンスタントバッファを登録する
    setVSConstantBuffers(constantBufferIndex);
    setPSConstantBuffers(constantBufferIndex);

    D3D11_MAPPED_SUBRESOURCE mapRes = { 0 };
    //開く
    if (map(&mapRes, constantBufferIndex)) {
        //データ転送
        memcpy_s(mapRes.pData, mapRes.RowPitch, data, size);
        //閉じる
        unmap(constantBufferIndex);
    }
}

void Shader::setVSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) const {
    MyDirectX::DirectX::instance().deviceContext()->VSSetShader(mVertexShader.Get(), &classInstances, numClassInstances);
}

void Shader::setPSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) const {
    MyDirectX::DirectX::instance().deviceContext()->PSSetShader(mPixelShader.Get(), &classInstances, numClassInstances);
}

void Shader::setVSConstantBuffers(unsigned index, unsigned numBuffers) const {
    MyDirectX::DirectX::instance().deviceContext()->VSSetConstantBuffers(index, numBuffers, mConstantBuffers[index]->bufferAddres());
}

void Shader::setPSConstantBuffers(unsigned index, unsigned numBuffers) const {
    MyDirectX::DirectX::instance().deviceContext()->PSSetConstantBuffers(index, numBuffers, mConstantBuffers[index]->bufferAddres());
}

void Shader::setInputLayout() const {
    MyDirectX::DirectX::instance().deviceContext()->IASetInputLayout(mVertexLayout->layout());
}

const std::string& Shader::getShaderName() const {
    return mShaderName;
}

void Shader::createVertexShader(const std::string& fileName) {
    //シェーダーファイルをコンパイルする
    if (!compileShader(&mVSBlob, fileName, "VS", "vs_5_0")) {
        return;
    }

    //ピクセルシェーダー作成
    auto res = MyDirectX::DirectX::instance().device()->CreateVertexShader(
        mVSBlob->GetBufferPointer(),
        mVSBlob->GetBufferSize(),
        nullptr,
        &mVertexShader
    );

    //失敗していたらエラーメッセージ発行
    if (FAILED(res)) {
        Debug::windowMessage(fileName + ": バーテックスシェーダー作成失敗");
    }
}

void Shader::createPixelShader(const std::string& fileName) {
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;

    //シェーダーファイルをコンパイルする
    if (!compileShader(&psBlob, fileName, "PS", "ps_5_0")) {
        return;
    }

    //ピクセルシェーダー作成
    auto res = MyDirectX::DirectX::instance().device()->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &mPixelShader
    );

    //失敗していたらエラーメッセージ発行
    if (FAILED(res)) {
        Debug::windowMessage(fileName + ": ピクセルシェーダー作成失敗");
    }
}

bool Shader::compileShader(Microsoft::WRL::ComPtr<ID3DBlob>* out, const std::string& fileName, const std::string& entryPoint, const std::string& target) {
    //Unicodeへ変換する
    auto wcharFileName = StringUtil::charToWchar(fileName);

    //コンパイル開始
    auto hresult = D3DCompileFromFile(
        wcharFileName.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint.c_str(),
        target.c_str(),
#ifdef _DEBUG
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
        0,
#endif // _DEBUG
        0,
        &*out,
        nullptr
    );

    //失敗していたらエラーメッセージ発行
    auto res = SUCCEEDED(hresult);
    if (!res) {
        Debug::windowMessage(fileName + ": hlsl読み込み失敗");
    }

    return res;
}

void Shader::createInputLayout(const std::vector<InputElementDesc>& layout) {
    mVertexLayout = std::make_unique<InputElement>(layout, mVSBlob.Get());
}

bool Shader::map(D3D11_MAPPED_SUBRESOURCE* mapRes, unsigned index, unsigned sub, D3D11_MAP type, unsigned flag) const {
    return SUCCEEDED(MyDirectX::DirectX::instance().deviceContext()->Map(mConstantBuffers[index]->buffer(), sub, type, flag, mapRes));
}

void Shader::unmap(unsigned index, unsigned sub) const {
    MyDirectX::DirectX::instance().deviceContext()->Unmap(mConstantBuffers[index]->buffer(), sub);
}
