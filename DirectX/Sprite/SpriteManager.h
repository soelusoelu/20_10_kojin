#pragma once

#include "../Math/Math.h"
#include <memory>
#include <list>

class Sprite3D;
class SpriteComponent;

class SpriteManager {
    using SpriteComponentPtr = std::shared_ptr<SpriteComponent>;
    using SpriteComponentPtrList = std::list<SpriteComponentPtr>;
    using Sprite3DPtr = std::shared_ptr<Sprite3D>;
    using Sprite3DPtrList = std::list<Sprite3DPtr>;

public:
    SpriteManager();
    ~SpriteManager();
    void update();
    void drawComponents(const Matrix4& proj) const;
    void draw3Ds(const Matrix4& view, const Matrix4& proj) const;
    void addComponent(const SpriteComponentPtr& add);
    void add3D(const Sprite3DPtr& add);
    void clear();

private:
    void remove();

    SpriteManager(const SpriteManager&) = delete;
    SpriteManager& operator=(const SpriteManager&) = delete;

private:
    SpriteComponentPtrList mSpriteComponents;
    Sprite3DPtrList mSprite3Ds;
};
