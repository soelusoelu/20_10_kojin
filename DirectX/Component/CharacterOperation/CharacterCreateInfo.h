#pragma once

#include <memory>
#include <string>

class SpriteComponent;

//キャラクター生成時情報
struct CharacterCreateInfo {
    std::string fileName;
    std::string spriteFileName;
    int cost;

    std::shared_ptr<SpriteComponent> sprite;
    bool isActive; //残りのコストをオーバーしてるか
};
