#include "TextureFromFile.h"
#include "../../DebugLayer/Debug.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../System/SystemInclude.h"
#include <DirectXTex.h>

TextureFromFile::TextureFromFile(const std::string& filePath) :
    Texture() {
    createTextureFromFileName(filePath);
}

TextureFromFile::~TextureFromFile() = default;

void TextureFromFile::createTextureFromFileName(const std::string& filePath) {
    //COMの初期化
    if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED))) {
        Debug::windowMessage("TextureFromFileクラスのCOM初期化に失敗しました");
        return;
    }

    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImage{};

    //Unicodeへ変換する
    wchar_t dst[256];
    MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, dst, _countof(dst));

    //ファイルからテクスチャ情報を取得
    if (FAILED(DirectX::LoadFromWICFile(dst, DirectX::WIC_FLAGS_NONE, &metadata, scratchImage))) {
        Debug::windowMessage(filePath + ": テクスチャ読み込み失敗");
        return;
    }

    //生データ抽出
    const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);

    //テクスチャサイズの設定
    mTextureSize = Vector2(metadata.width, metadata.height);

    ID3D11ShaderResourceView* srv = nullptr;
    if (FAILED(DirectX::CreateShaderResourceView(MyDirectX::DirectX::instance().device(), image, 1, metadata, &srv))) {
        Debug::windowMessage(filePath + ": テクスチャ作成失敗");
    }
    mShaderResourceView = std::make_unique<ShaderResourceView>(srv);
}
