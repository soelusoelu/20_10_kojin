#include "PlayerMove.h"
#include "../../Device/Time.h"
#include "../../Input/Input.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

PlayerMove::PlayerMove(GameObject& gameObject) :
    Component(gameObject),
    mMoveSpeed(0.f)
{
}

PlayerMove::~PlayerMove() = default;

void PlayerMove::start() {
}

void PlayerMove::update() {
    if (Input::keyboard().getKey(KeyCode::Q)) {
        transform().rotate(Vector3::up, -60.f * Time::deltaTime);
    }
    if (Input::keyboard().getKey(KeyCode::E)) {
        transform().rotate(Vector3::up, 60.f * Time::deltaTime);
    }

    auto dir = Vector3::normalize(Vector3(
        Input::keyboard().horizontal(),
        0.f,
        Input::keyboard().vertical()
    ));
    transform().translate(dir * mMoveSpeed * Time::deltaTime);
}

void PlayerMove::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "moveSpeed", &mMoveSpeed);
}

void PlayerMove::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("MoveSpeed", mMoveSpeed);
}
