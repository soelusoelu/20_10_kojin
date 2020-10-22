#pragma once

#include "Texture.h"
#include "../../Math/Math.h"
#include <vector>

class TextureFromMemory : public Texture {
public:
    TextureFromMemory(unsigned width, unsigned height);
    TextureFromMemory(const std::vector<unsigned char>& data, unsigned width, unsigned height);
    ~TextureFromMemory();

    //テクスチャを単色で塗りつぶす
    void clear();
    //指定した座標の色を設定する [0, 1]
    void setPixel(unsigned x, unsigned y, const Vector3& color);
    //指定した座標の色を設定する [0, 255]
    void setPixel(unsigned x, unsigned y, unsigned char r, unsigned char g, unsigned char b);
    //clear, setPixel関数での変更を適用する
    void apply();

private:
    TextureFromMemory(const TextureFromMemory&) = delete;
    TextureFromMemory& operator=(const TextureFromMemory&) = delete;

    //メモリデータからテクスチャを作成する
    void createTextureFromMemory(unsigned char* data, unsigned width, unsigned height);

private:
    std::vector<unsigned char> mPixelData;
    static constexpr int PIXEL_DATA_SIZE = sizeof(unsigned char) * 4;
};
