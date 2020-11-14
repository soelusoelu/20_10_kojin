#include "Title.h"
#include "Scene.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

Title::Title(GameObject& gameObject) :
    Component(gameObject),
    mScene(nullptr)
{
}

Title::~Title() = default;

void Title::start() {
    mScene = getComponent<Scene>();
    GameObjectCreater::create("SkinMesh");
    GameObjectCreater::create("Plane");
    //GameObjectCreater::create("SkinMeshPole");
}

void Title::update() {
    if (Input::keyboard().getEnter()) {
        mScene->next("GamePlay");
    }

    Debug::renderLine(Vector3::left * 100.f, Vector3::right * 100.f, ColorPalette::red);
    Debug::renderLine(Vector3::down * 100.f, Vector3::up * 100.f, ColorPalette::green);
    Debug::renderLine(Vector3::back * 100.f, Vector3::forward * 100.f, ColorPalette::blue);
}
