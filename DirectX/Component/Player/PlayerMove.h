#pragma once

#include "../Component.h"

class PlayerMove : public Component {
public:
    PlayerMove(GameObject& gameObject);
    ~PlayerMove();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

private:
    float mMoveSpeed;
};
