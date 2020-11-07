#include "JoyPad.h"
#include "InputUtility.h"
#include "../Math/Math.h"
#include "../System/GlobalFunction.h"
#include "../Utility/LevelLoader.h"

BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE*, VOID*);
BOOL CALLBACK enumObjectsCallback(const DIDEVICEOBJECTINSTANCE*, VOID*);

JoyPad::JoyPad() :
    mCurrentJoyState(),
    mPreviousJoyState(),
    mEnterPad(JoyCode::None) {
}

JoyPad::~JoyPad() {
    safeRelease<IDirectInputDevice8>(mPadDevice);
}

bool JoyPad::initialize(const HWND& hWnd, IDirectInput8* directInput) {
    //利用可能なゲームコントローラーの列挙関数を実行
    if (FAILED(directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY))) {
        return false;
    }
    if (!mPadDevice) { //パッドの接続はせずに終了
        return true;
    }
    // デバイスをジョイスティックに設定
    if (FAILED(mPadDevice->SetDataFormat(&c_dfDIJoystick2))) {
        return false;
    }
    // 協調レベルの設定
    if (FAILED(mPadDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
        return false;
    }
    //アプリケーションで使用するコントローラーのプロパティを列挙して設定する
    if (FAILED(mPadDevice->EnumObjects(enumObjectsCallback, NULL, DIDFT_ALL))) {
        return false;
    }
    // デバイスを「取得」する
    mPadDevice->Acquire();

    return true;
}

void JoyPad::loadProperties(const rapidjson::Value& inObj) {
    std::string src;
    if (JsonHelper::getString(inObj, "enterPad", &src)) {
        stringToJoyCode(src, &mEnterPad);
    }
}

BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
    // 「DirectInputデバイス」オブジェクトの作成
    if (FAILED(InputUtility::mDirectInput->CreateDevice(pdidInstance->guidInstance, &JoyPad::mPadDevice, NULL))) {
        return DIENUM_CONTINUE;
    }

    return DIENUM_STOP;
}

BOOL CALLBACK enumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) {
    if (pdidoi->dwType & DIDFT_AXIS) {
        DIPROPRANGE diprg;
        diprg.diph.dwSize = sizeof(DIPROPRANGE);
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        diprg.diph.dwHow = DIPH_BYID;
        diprg.diph.dwObj = pdidoi->dwType;
        diprg.lMin = -JoyPad::STICK_VALUE;
        diprg.lMax = JoyPad::STICK_VALUE;

        if (FAILED(JoyPad::mPadDevice->SetProperty(DIPROP_RANGE, &diprg.diph))) {
            return DIENUM_STOP;
        }
    }
    return DIENUM_CONTINUE;
}

void JoyPad::update() {
    if (mPadDevice) {
        mPreviousJoyState = mCurrentJoyState;

        HRESULT hr = mPadDevice->Acquire();
        if ((hr == DI_OK) || (hr == S_FALSE)) {
            mPadDevice->GetDeviceState(sizeof(DIJOYSTATE2), &mCurrentJoyState);
        }
    }
}

bool JoyPad::getJoyDown(JoyCode joy) const {
    return (mCurrentJoyState.rgbButtons[static_cast<int>(joy)] & 0x80 && !(mPreviousJoyState.rgbButtons[static_cast<int>(joy)] & 0x80));
}

bool JoyPad::getJoy(JoyCode joy) const {
    return mCurrentJoyState.rgbButtons[static_cast<int>(joy)] & 0x80;
}

bool JoyPad::getJoyUp(JoyCode joy) const {
    return (!(mCurrentJoyState.rgbButtons[static_cast<int>(joy)] & 0x80) && mPreviousJoyState.rgbButtons[static_cast<int>(joy)] & 0x80);
}

Vector2 JoyPad::leftStick() const {
    Vector2 temp = Vector2::zero;
    temp.x = (Math::abs(mCurrentJoyState.lX) > DEAD_ZONE) ? mCurrentJoyState.lX / STICK_VALUE : 0.f;
    temp.y = (Math::abs(mCurrentJoyState.lY) > DEAD_ZONE) ? -mCurrentJoyState.lY / STICK_VALUE : 0.f;

    return temp;
}

Vector2 JoyPad::rightStick() const {
    Vector2 temp = Vector2::zero;
    temp.x = (Math::abs(mCurrentJoyState.lRx) > DEAD_ZONE) ? mCurrentJoyState.lRx / STICK_VALUE : 0.f;
    temp.y = (Math::abs(mCurrentJoyState.lRx) > DEAD_ZONE) ? -mCurrentJoyState.lRx / STICK_VALUE : 0.f;

    return temp;
}

bool JoyPad::getEnter() const {
    return getJoyDown(mEnterPad);
}

void JoyPad::stringToJoyCode(const std::string& src, JoyCode* dst) {
    auto joy = JoyCode::None;

    if (src == "A") {
        joy = JoyCode::A;
    } else if (src == "B") {
        joy = JoyCode::B;
    } else if (src == "X") {
        joy = JoyCode::X;
    } else if (src == "Y") {
        joy = JoyCode::Y;
    } else if (src == "LeftButton") {
        joy = JoyCode::LeftButton;
    } else if (src == "RightButton") {
        joy = JoyCode::RightButton;
    } else if (src == "Select") {
        joy = JoyCode::Select;
    } else if (src == "Start") {
        joy = JoyCode::Start;
    } else if (src == "LeftStickButton") {
        joy = JoyCode::LeftStickButton;
    } else if (src == "RightStickButton") {
        joy = JoyCode::RightStickButton;
    }

    if (joy != JoyCode::None) {
        *dst = joy;
    }
}

IDirectInputDevice8* JoyPad::mPadDevice = nullptr;
