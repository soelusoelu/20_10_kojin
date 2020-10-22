#pragma once

#include "../Math/Math.h"
#include <memory>
#include <string>

class TextureFromFile;

struct Material {
    Vector3 ambient; //アンビエント
    Vector3 diffuse; //ディフューズ
    Vector3 specular; //スペキュラー
    Vector3 emissive;
    Vector3 bump;
    float transparency; //透明値
    float shininess;
    std::string materialName; //マテリアル名
    std::shared_ptr<TextureFromFile> texture;
    std::shared_ptr<TextureFromFile> mapTexture;

    Material() :
        ambient(Vector3::zero),
        diffuse(Vector3::one),
        specular(Vector3::zero),
        emissive(Vector3::zero),
        bump(Vector3::zero),
        transparency(1.f),
        shininess(1.f),
        materialName(),
        texture(nullptr),
        mapTexture(nullptr) {
    }
};
