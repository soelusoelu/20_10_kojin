#include "Pause.h"
#include "Button.h"
#include "../Input/Input.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

Pause::Pause() :
    mButton(nullptr),
    mFileName(""),
    mOffset(Vector2::zero),
    mIsPausing(false) {
}

Pause::~Pause() = default;

void Pause::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["pause"];
    if (obj.IsObject()) {
        JsonHelper::getString(obj, "fileName", &mFileName);
        JsonHelper::getVector2(obj, "offset", &mOffset);
    }
}

void Pause::initialize() {
    auto pos = mOffset;
    pos.x += Window::width();
    mButton = std::make_unique<SpriteButton>(nullptr, mFileName, pos);
}

void Pause::update() {
    if (!Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        return;
    }

    const auto& mousePos = Input::mouse().getMousePosition();
    if (!mButton->containsPoint(mousePos)) {
        return;
    }

    //ボタンがクリックされた
    mIsPausing = !mIsPausing;
}

bool Pause::isPausing() const {
    return mIsPausing;
}

void Pause::drawButton(const Matrix4& proj) const {
    mButton->draw(proj);
}
