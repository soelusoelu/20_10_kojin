#pragma once

#include "../Component.h"
#include <memory>

class Scene;

class Title : public Component {
public:
    Title(GameObject& gameObject);
    ~Title();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<Scene> mScene;
};
