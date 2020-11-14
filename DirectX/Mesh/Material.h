#pragma once

#include "../Math/Math.h"
#include <memory>
#include <string>

class Texture;

struct Material {
    Vector3 ambient; //アンビエント
    Vector3 diffuse; //ディフューズ
    Vector3 specular; //スペキュラー
    Vector3 emissive;
    Vector3 bump;
    float transparency; //透明値
    float shininess;
    std::string materialName; //マテリアル名
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Texture> normalMapTexture;

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
        normalMapTexture(nullptr) {
    }
};
