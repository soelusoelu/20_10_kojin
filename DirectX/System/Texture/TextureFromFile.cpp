#include "TextureFromFile.h"
#include "../../DebugLayer/Debug.h"
#include "../../DirectX/DirectXInclude.h"

TextureFromFile::TextureFromFile(const std::string& fileName) :
    Texture() {
    createTextureFromFileName(fileName);
}

TextureFromFile::~TextureFromFile() = default;

void TextureFromFile::createTextureFromFileName(const std::string& fileName) {
    //ファイルからテクスチャ情報を取得
    D3DX11_IMAGE_INFO info{};
    D3DX11GetImageInfoFromFileA(fileName.c_str(), nullptr, &info, nullptr);

    mTextureSize = Vector2(info.Width, info.Height);

    D3DX11_IMAGE_LOAD_INFO loadInfo{};
    loadInfo.Width = info.Width;
    loadInfo.Height = info.Height;
    loadInfo.Depth = info.Depth;
    loadInfo.MipLevels = info.MipLevels;
    loadInfo.MiscFlags = info.MiscFlags;
    loadInfo.Format = info.Format;
    loadInfo.FirstMipLevel = 0;
    loadInfo.Usage = toUsage(Usage::USAGE_DEFAULT);
    loadInfo.BindFlags = static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_SHADER_RESOURCE);
    loadInfo.CpuAccessFlags = 0;
    loadInfo.Filter = D3DX11_FILTER_LINEAR; //線形補間
    loadInfo.MipFilter = D3DX11_FILTER_LINEAR; //線形補間
    loadInfo.pSrcInfo = nullptr;

    ID3D11ShaderResourceView* srv = nullptr;
    if (FAILED(D3DX11CreateShaderResourceViewFromFileA(DirectX::instance().device(), fileName.c_str(), &loadInfo, nullptr, &srv, nullptr))) {
        Debug::windowMessage(fileName + ": テクスチャ作成失敗");
    }
    mShaderResourceView = std::make_unique<ShaderResourceView>(srv);
}
