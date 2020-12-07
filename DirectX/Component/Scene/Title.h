#pragma once

#include "Scene.h"

class Title : public Scene {
public:
    Title(GameObject& gameObject);
    ~Title();
    virtual void update() override;
};
