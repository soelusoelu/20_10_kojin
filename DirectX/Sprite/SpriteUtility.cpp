#include "SpriteUtility.h"
#include "../Collision/Collision.h"
#include "../Component/Sprite/SpriteComponent.h"
#include "../Sprite/Sprite.h"
#include "../System/Window.h"
#include "../Transform/Transform2D.h"

bool SpriteUtility::contains(const Sprite& sprite, const Vector2& position) {
    return containsTexture(sprite.transform(), sprite.getTextureSize(), position);
}

bool SpriteUtility::contains(const SpriteComponent& sprite, const Vector2& position) {
    return containsTexture(sprite.transform(), sprite.getTextureSize(), position);
}

bool SpriteUtility::containsTexture(const Transform2D& transform, const Vector2& textureSize, const Vector2& targetPosition) {
    //ウィンドウ補正値を取得する
    auto compen = Window::getWindowCompensate();

    //計算に必要な要素を取得する
    const auto& scale = transform.getScale();
    const auto& pivot = transform.getPivot() * scale;
    const auto& compenPos = (transform.getPosition() - pivot) * compen;
    const auto& compenScale = scale * compen;
    const auto& texSize = textureSize * compenScale;

    //スプライトをもとに矩形作成
    Square square(compenPos, compenPos + texSize);

    //矩形の中にマウスの座標が含まれているか
    return square.contains(targetPosition);
}
