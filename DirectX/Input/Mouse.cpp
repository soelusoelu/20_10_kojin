#include "Mouse.h"
#include "../Math/Math.h"
#include "../System/GlobalFunction.h"
#include "../System/Window.h"

Mouse::Mouse() :
    mMouseDevice(nullptr),
    mCurrentMouseState(),
    mPreviousMouseState(),
    mhWnd(nullptr),
    mMousePosition(Vector2::zero) {
}

Mouse::~Mouse() {
    safeRelease<IDirectInputDevice8>(mMouseDevice);
}

bool Mouse::getMouseButtonDown(MouseCode button) {
    return (mCurrentMouseState.rgbButtons[static_cast<int>(button)] & 0x80 && !(mPreviousMouseState.rgbButtons[static_cast<int>(button)] & 0x80));
}

bool Mouse::getMouseButton(MouseCode button) {
    return mCurrentMouseState.rgbButtons[static_cast<int>(button)] & 0x80;
}

bool Mouse::getMouseButtonUp(MouseCode button) {
    return (!(mCurrentMouseState.rgbButtons[static_cast<int>(button)] & 0x80) && mPreviousMouseState.rgbButtons[static_cast<int>(button)] & 0x80);
}

const Vector2& Mouse::getMousePosition() {
    return mMousePosition;
}

bool Mouse::initialize(HWND hWnd, IDirectInput8* directInput) {
    mhWnd = hWnd;

    //「DirectInputデバイス」オブジェクトの作成
    if (FAILED(directInput->CreateDevice(GUID_SysMouse, &mMouseDevice, NULL))) {
        return false;
    }
    //デバイスをマウスに設定
    if (FAILED(mMouseDevice->SetDataFormat(&c_dfDIMouse2))) {
        return false;
    }
    //協調レベルの設定
    if (FAILED(mMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
        return false;
    }
    //デバイスを「取得」する
    mMouseDevice->Acquire();

    return true;
}

void Mouse::update() {
    mPreviousMouseState = mCurrentMouseState;

    HRESULT hr = mMouseDevice->Acquire();
    if ((hr == DI_OK) || (hr == S_FALSE)) {
        mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mCurrentMouseState);
    }

    updateMousePosition();
    clampMousePosition();
}

void Mouse::stringToJoyCode(const std::string& src, MouseCode* dst) {
    auto key = MouseCode::None;

    if (src == "LeftButton") {
        key = MouseCode::LeftButton;
    } else if (src == "RightButton") {
        key = MouseCode::RightButton;
    } else if (src == "CenterButton") {
        key = MouseCode::CenterButton;
    } else if (src == "SideButton1") {
        key = MouseCode::SideButton1;
    } else if (src == "SideButton2") {
        key = MouseCode::SideButton2;
    } else if (src == "SideButton3") {
        key = MouseCode::SideButton3;
    }

    if (key != MouseCode::None) {
        *dst = key;
    }
}

void Mouse::updateMousePosition() {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(mhWnd, &point);
    mMousePosition.x = static_cast<float>(point.x);
    mMousePosition.y = static_cast<float>(point.y);
    mMousePosition.x *= Window::windowToClientSize().x;
    mMousePosition.y *= Window::windowToClientSize().y;
}

void Mouse::clampMousePosition() {
#ifdef _DEBUG
    mMousePosition.clamp(
        Vector2::zero,
        Vector2(static_cast<float>(Window::debugWidth()), static_cast<float>(Window::debugHeight()))
    );
#else
    mMousePosition.clamp(
        Vector2::zero,
        Vector2(static_cast<float>(Window::width()), static_cast<float>(Window::height()))
    );
#endif // _DEBUG
}
