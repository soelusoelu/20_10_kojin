#include "Title.h"
#include "../../Input/Input.h"

Title::Title(GameObject& gameObject) :
    Scene(gameObject)
{
}

Title::~Title() = default;

void Title::update() {
    if (Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        next("GamePlay");
    }
}
