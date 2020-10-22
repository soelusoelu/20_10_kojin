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

    //進みたい方向
    auto dir = Vector3::normalize(Vector3(
        Input::keyboard().horizontal(),
        0.f,
        Input::keyboard().vertical()
    ));
    //移動
    transform().translate((dir.x * transform().right() + dir.z * transform().forward()) * mMoveSpeed * Time::deltaTime);
}

void PlayerMove::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "moveSpeed", &mMoveSpeed);
}

void PlayerMove::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("MoveSpeed", mMoveSpeed);
}
