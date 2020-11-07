#include "InputUtility.h"
#include "JoyPad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "../System/GlobalFunction.h"

void InputUtility::create() {
    mKeyboard = new Keyboard();
    mMouse = new Mouse();
    mJoyPad = new JoyPad();
}

bool InputUtility::initialize(const HWND& hWnd) {
    //「DirectInput」オブジェクトの作成
    if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, nullptr))) {
        return false;
    }

    mKeyboard->initialize(hWnd, mDirectInput);
    mMouse->initialize(hWnd, mDirectInput);
    mJoyPad->initialize(hWnd, mDirectInput);

    return true;
}

void InputUtility::loadProperties(const rapidjson::Value& inObj) {
    mKeyboard->loadProperties(inObj);
    mJoyPad->loadProperties(inObj);
}

void InputUtility::finalize() {
    safeRelease(mDirectInput);
    safeDelete(mKeyboard);
    safeDelete(mMouse);
    safeDelete(mJoyPad);
}

void InputUtility::update() {
    mKeyboard->update();
    mMouse->update();
    mJoyPad->update();
}

Keyboard& InputUtility::keyboard() {
    return *mKeyboard;
}

Mouse& InputUtility::mouse() {
    return *mMouse;
}

JoyPad& InputUtility::joyPad() {
    return *mJoyPad;
}
