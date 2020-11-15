#pragma once

#include "../../Math/Math.h"
#include <memory>
#include <string>

//頂点の構造体
struct TextureVertex {
    Vector3 pos; //位置
    Vector2 uv; //テクスチャ座標
};

class VertexBuffer;
class IndexBuffer;
class ShaderResourceView;
class Sampler;

class Texture {
public:
    Texture();
    virtual ~Texture();
    //終了処理
    static void finalize();
    //テクスチャサイズを返す
    const Vector2& getTextureSize() const;
    //テクスチャの登録
    void setVSTextures(unsigned start = 0, unsigned numTextures = 1) const;
    void setPSTextures(unsigned start = 0, unsigned numTextures = 1) const;
    //サンプラーの登録
    void setVSSamplers(unsigned start = 0, unsigned numSamplers = 1) const;
    void setPSSamplers(unsigned start = 0, unsigned numSamplers = 1) const;
    //描画に必要な要素をすべて登録する
    void setTextureInfo(unsigned start = 0, unsigned numSamplers = 1) const;

private:
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void createVertexBuffer();
    void createIndexBuffer();
    void createSampler();

public:
    static inline VertexBuffer* vertexBuffer = nullptr;
    static inline VertexBuffer* vertexBuffer3D = nullptr;
    static inline IndexBuffer* indexBuffer = nullptr;

protected:
    std::unique_ptr<ShaderResourceView> mShaderResourceView;
    std::unique_ptr<Sampler> mSampler;
    Vector2 mTextureSize;
};
