#include "GamePlay.h"
#include "Scene.h"
#include "../ComponentManager.h"
#include "../Camera/LookAtObject.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"

GamePlay::GamePlay(GameObject& gameObject) :
    Component(gameObject),
    mScene(nullptr) {
}

GamePlay::~GamePlay() = default;

void GamePlay::start() {
    mScene = getComponent<Scene>();

    //プレイヤー
    auto playerObj = GameObjectCreater::create("Player");

    //プレイヤーを追跡する
    auto lookAtObj = GameObjectCreater::create("LookAtObject");
    auto lookAtComp = lookAtObj->componentManager().getComponent<LookAtObject>();
    lookAtComp->setTarget(playerObj);

    //地面(背景)
    GameObjectCreater::create("Ground");
}

void GamePlay::update() {
#ifdef _DEBUG
    //リセット
    if (Input::keyboard().getKeyDown(KeyCode::R)) {
        mScene->next("GamePlay");
    }
#endif // _DEBUG
}
