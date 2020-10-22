#include "Hierarchy.h"
#include "Button.h"
#include "../DebugLayer/DebugUtility.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/DrawString.h"
#include "../GameObject/GameObject.h"
#include "../Input/Input.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

Hierarchy::Hierarchy(DrawString* drawString) :
    mDrawString(drawString),
    mNumRowsToDisplay(0),
    mLineSpace(0.f),
    mInspectorPositionX(0.f),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mOffsetCharCountX(0),
    mOffsetCharCountY(0),
    mCharWidth(0.f),
    mCharHeight(0.f),
    mNonActiveAlpha(0.5f) {
}

Hierarchy::~Hierarchy() = default;

void Hierarchy::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["hierarchy"];
    if (obj.IsObject()) {
        JsonHelper::getVector2(obj, "scale", &mScale);
        JsonHelper::getInt(obj, "offsetCharCountX", &mOffsetCharCountX);
        JsonHelper::getInt(obj, "offsetCharCountY", &mOffsetCharCountY);
        JsonHelper::getFloat(obj, "lineSpace", &mLineSpace);
        JsonHelper::getFloat(obj, "nonActiveAlpha", &mNonActiveAlpha);
    }
    const auto& inspector = inObj["inspector"];
    if (inspector.IsObject()) {
        JsonHelper::getFloat(inspector, "inspectorPositionX", &mInspectorPositionX);
    }
}

void Hierarchy::initialize() {
    mCharWidth = DrawString::WIDTH * mScale.x;
    mCharHeight = DrawString::HEIGHT * mScale.y;

    mPosition = Vector2(Window::width(), 0.f);
    mPosition += Vector2(mOffsetCharCountX * mCharWidth, mOffsetCharCountY * mCharHeight);

    //画面内に収まる行数
    mNumRowsToDisplay = (Window::debugHeight() - mPosition.y) / (mCharHeight + mLineSpace);

    mButtons.resize(mNumRowsToDisplay);
    auto pos = mPosition;
    for (auto&& b : mButtons) {
        //全ボタンに当たり判定をつける
        b.first = std::make_unique<Button>(nullptr, pos, Vector2(mInspectorPositionX - pos.x, mCharHeight));
        b.second = nullptr;
        pos.y += mCharHeight + mLineSpace;
    }
}

void Hierarchy::setGameObjectToButton(const GameObjectPtrList& gameObjects) {
    for (auto&& b : mButtons) {
        b.second.reset();
    }

    auto itr = mButtons.begin();
    for (const auto& obj : gameObjects) {
        //オブジェクトの数がボタンの数より多いときは無視
        if (itr == mButtons.end()) {
            return;
        }
        itr->second = obj;
        ++itr;
    }
}

void Hierarchy::update() {
    const auto& mousePos = Input::mouse().getMousePosition();
    if (Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        for (const auto& b : mButtons) {
            if (!b.first->containsPoint(mousePos)) {
                continue;
            }
            if (b.second) {
                DebugUtility::inspector().setTarget(b.second);
                break;
            }
        }
    }
}

void Hierarchy::drawActors() const {
    for (const auto& b : mButtons) {
        auto obj = b.second;
        //オブジェクトが登録されてなかったら終了
        if (!obj) {
            break;
        }

        float alpha = 1.f;
        if (!obj->getActive()) {
            alpha = mNonActiveAlpha;
        }
        mDrawString->drawString(obj->tag(), b.first->getPosition(), mScale, ColorPalette::white, alpha);
    }
}
