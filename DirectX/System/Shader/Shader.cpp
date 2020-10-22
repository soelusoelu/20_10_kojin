#include "Shader.h"
#include "ConstantBufferManager.h"
#include "InputElementManager.h"
#include "../GlobalFunction.h"
#include "../../DebugLayer/Debug.h"
#include "../../DirectX/DirectXInclude.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

Shader::Shader(const std::string& fileName) :
    mVSBlob(nullptr),
    mVertexShader(nullptr),
    mPixelShader(nullptr),
    mVertexLayout(nullptr) {
    if (!constantBufferManager || !inputElementManager) {
        constantBufferManager = new ConstantBufferManager();
        inputElementManager = new InputElementManager();
    }
    createVertexShader(fileName);
    createPixelShader(fileName);
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

    //開く
    D3D11_MAPPED_SUBRESOURCE mapRes = { 0 };
    map(&mapRes, constantBufferIndex);
    //データ転送
    memcpy_s(mapRes.pData, mapRes.RowPitch, data, size);
    //閉じる
    unmap(constantBufferIndex);
}

void Shader::setVSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) const {
    DirectX::instance().deviceContext()->VSSetShader(mVertexShader.Get(), &classInstances, numClassInstances);
}

void Shader::setPSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) const {
    DirectX::instance().deviceContext()->PSSetShader(mPixelShader.Get(), &classInstances, numClassInstances);
}

void Shader::setVSConstantBuffers(unsigned index, unsigned numBuffers) const {
    auto buf = mConstantBuffers[index]->buffer();
    DirectX::instance().deviceContext()->VSSetConstantBuffers(index, numBuffers, &buf);
}

void Shader::setPSConstantBuffers(unsigned index, unsigned numBuffers) const {
    auto buf = mConstantBuffers[index]->buffer();
    DirectX::instance().deviceContext()->PSSetConstantBuffers(index, numBuffers, &buf);
}

void Shader::setInputLayout() const {
    DirectX::instance().deviceContext()->IASetInputLayout(mVertexLayout->layout());
}

void Shader::createVertexShader(const std::string& fileName) {
    //シェーダーファイルをコンパイルする
    if (!compileShader(&mVSBlob, fileName, "VS", "vs_5_0")) {
        return;
    }

    //ピクセルシェーダー作成
    auto res = DirectX::instance().device()->CreateVertexShader(
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
    auto res = DirectX::instance().device()->CreatePixelShader(
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
    wchar_t dst[256];
    MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, dst, _countof(dst));

    //コンパイル開始
    auto hresult = D3DCompileFromFile(
        dst,
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

void Shader::map(D3D11_MAPPED_SUBRESOURCE* mapRes, unsigned index, unsigned sub, D3D11_MAP type, unsigned flag) const {
    DirectX::instance().deviceContext()->Map(mConstantBuffers[index]->buffer(), sub, type, flag, mapRes);
}

void Shader::unmap(unsigned index, unsigned sub) const {
    DirectX::instance().deviceContext()->Unmap(mConstantBuffers[index]->buffer(), sub);
}
