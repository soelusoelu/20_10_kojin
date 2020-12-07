#include "TextureFromMemory.h"
#include "../../DirectX/DirectXInclude.h"
#include <algorithm>

TextureFromMemory::TextureFromMemory(unsigned width, unsigned height) :
    Texture(),
    mPixelData(width * height * PIXEL_DATA_SIZE, 255) {
    createTextureFromMemory(mPixelData.data(), width, height);
}

TextureFromMemory::TextureFromMemory(const std::vector<unsigned char>& data, unsigned width, unsigned height) :
    Texture(),
    mPixelData(width * height * PIXEL_DATA_SIZE) {
    std::copy(data.begin(), data.end(), mPixelData.begin());
    createTextureFromMemory(mPixelData.data(), width, height);
}

TextureFromMemory::~TextureFromMemory() = default;

void TextureFromMemory::clear() {
    std::fill(mPixelData.begin(), mPixelData.end(), 255);
}

void TextureFromMemory::setPixel(unsigned x, unsigned y, const Vector3 & color) {
    setPixel(x, y, color.x * 255, color.y * 255, color.z * 255);
}

void TextureFromMemory::setPixel(unsigned x, unsigned y, unsigned char r, unsigned char g, unsigned char b) {
    //データがunsigned char型で色3つアルファ値1つ並んでるから
    auto p = y * mTextureSize.x * PIXEL_DATA_SIZE + x * PIXEL_DATA_SIZE;
    mPixelData[p] = r;
    mPixelData[p + 1] = g;
    mPixelData[p + 2] = b;
}

void TextureFromMemory::apply() {
    createTextureFromMemory(mPixelData.data(), mTextureSize.x, mTextureSize.y);
}

void TextureFromMemory::createTextureFromMemory(unsigned char* data, unsigned width, unsigned height) {
    mTextureSize = Vector2(width, height);

    Texture2DDesc tex2DDesc;
    tex2DDesc.width = width;
    tex2DDesc.height = height;

    SubResourceDesc sub;
    sub.data = data;
    sub.pitch = width * PIXEL_DATA_SIZE;

    auto tex2D = std::make_unique<Texture2D>(tex2DDesc, &sub);

    mShaderResourceView = std::make_shared<ShaderResourceView>(*tex2D);
}
