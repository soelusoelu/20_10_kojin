#pragma once

class Camera;
class DirectionalLight;

//コールバック用メッシュ描画前インターフェース
class IDrawBefore {
public:
    virtual ~IDrawBefore() = default;
    //描画前にコールされる
    virtual void drawBefore(const Camera& camera, const DirectionalLight& dirLight) const = 0;
};
