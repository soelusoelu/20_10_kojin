#pragma once

#include "../Math/Math.h"

class Sprite;
class SpriteComponent;
class Transform2D;

//スプライト補助クラス
class SpriteUtility {
public:
    //スプライトの中に指定の座標が含まれているか
    static bool contains(const Sprite& sprite, const Vector2& position);
    //スプライトコンポーネントの中に指定の座標が含まれているか
    static bool contains(const SpriteComponent& sprite, const Vector2& position);

private:
    //トランスフォームと画像サイズから指定の座標が含まれているか調べる
    static bool containsTexture(const Transform2D& transform, const Vector2& textureSize, const Vector2& targetPosition);

private:
    SpriteUtility() = delete;
    ~SpriteUtility() = delete;
};
