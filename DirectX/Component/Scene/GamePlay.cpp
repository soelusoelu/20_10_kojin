#include "GamePlay.h"
#include "Scene.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"

GamePlay::GamePlay(GameObject& gameObject) :
    Component(gameObject),
    mScene(nullptr) {
}

GamePlay::~GamePlay() = default;

void GamePlay::start() {
    mScene = getComponent<Scene>();

    //エミッターを生成
    GameObjectCreater::create("Test");
    GameObjectCreater::create("Test2");
}

void GamePlay::update() {
#ifdef _DEBUG
    //リセット
    if (Input::keyboard().getKeyDown(KeyCode::R)) {
        mScene->next("GamePlay");
    }
#endif // _DEBUG
}
