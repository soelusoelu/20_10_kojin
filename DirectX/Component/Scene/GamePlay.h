#pragma once

#include "Scene.h"
#include <memory>

class GamePlay : public Scene {
public:
    GamePlay(GameObject& gameObject);
    ~GamePlay();
    virtual void start() override;
    virtual void update() override;

private:

};
