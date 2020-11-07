#include "Keyboard.h"
#include "../System/GlobalFunction.h"
#include "../Utility/LevelLoader.h"

Keyboard::Keyboard() :
    mKeyDevice(nullptr),
    mCurrentKeys(),
    mPreviousKeys(),
    mEnterKey(KeyCode::None) {
}

Keyboard::~Keyboard() {
    safeRelease<IDirectInputDevice8>(mKeyDevice);
}

bool Keyboard::getKeyDown(KeyCode key) const {
    return (mCurrentKeys[static_cast<BYTE>(key)] & 0x80 && !(mPreviousKeys[static_cast<BYTE>(key)] & 0x80));
}

bool Keyboard::getKey(KeyCode key) const {
    return mCurrentKeys[static_cast<BYTE>(key)] & 0x80;
}

bool Keyboard::getKeyUp(KeyCode key) const {
    return (!(mCurrentKeys[static_cast<BYTE>(key)] & 0x80) && mPreviousKeys[static_cast<BYTE>(key)] & 0x80);
}

int Keyboard::horizontal() const {
    if (getKey(KeyCode::A) || getKey(KeyCode::LeftArrow)) {
        return -1;
    } else if (getKey(KeyCode::D) || getKey(KeyCode::RightArrow)) {
        return 1;
    } else {
        return 0;
    }
}

int Keyboard::vertical() const {
    if (getKey(KeyCode::W) || getKey(KeyCode::UpArrow)) {
        return 1;
    } else if (getKey(KeyCode::S) || getKey(KeyCode::DownArrow)) {
        return -1;
    } else {
        return 0;
    }
}

bool Keyboard::getEnter() const {
    return getKeyDown(mEnterKey);
}

bool Keyboard::initialize(const HWND& hWnd, IDirectInput8* directInput) {
    //「DirectInputデバイス」オブジェクトの作成
    if (FAILED(directInput->CreateDevice(GUID_SysKeyboard, &mKeyDevice, NULL))) {
        return false;
    }
    //入力データ形式のセット
    if (FAILED(mKeyDevice->SetDataFormat(&c_dfDIKeyboard))) {
        return false;
    }
    //協調レベルの設定
    if (FAILED(mKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
        return false;
    }
    //デバイスを「取得」する
    mKeyDevice->Acquire();

    return true;
}

void Keyboard::loadProperties(const rapidjson::Value& inObj) {
    std::string src;
    if (JsonHelper::getString(inObj, "enterKey", &src)) {
        stringToKeyCode(src, &mEnterKey);
    }
}

void Keyboard::update() {
    memcpy_s(mPreviousKeys, sizeof(mPreviousKeys), mCurrentKeys, sizeof(mCurrentKeys));

    HRESULT hr = mKeyDevice->Acquire();
    if ((hr == DI_OK) || (hr == S_FALSE)) {
        mKeyDevice->GetDeviceState(sizeof(mCurrentKeys), &mCurrentKeys);
    }
}

void Keyboard::stringToKeyCode(const std::string& src, KeyCode* dst) {
    auto key = KeyCode::None;

    if (src == "A") {
        key = KeyCode::A;
    } else if (src == "B") {
        key = KeyCode::B;
    } else if (src == "C") {
        key = KeyCode::C;
    } else if (src == "D") {
        key = KeyCode::D;
    } else if (src == "E") {
        key = KeyCode::E;
    } else if (src == "F") {
        key = KeyCode::F;
    } else if (src == "G") {
        key = KeyCode::G;
    } else if (src == "H") {
        key = KeyCode::H;
    } else if (src == "I") {
        key = KeyCode::I;
    } else if (src == "J") {
        key = KeyCode::J;
    } else if (src == "K") {
        key = KeyCode::K;
    } else if (src == "L") {
        key = KeyCode::L;
    } else if (src == "M") {
        key = KeyCode::M;
    } else if (src == "N") {
        key = KeyCode::N;
    } else if (src == "O") {
        key = KeyCode::O;
    } else if (src == "P") {
        key = KeyCode::P;
    } else if (src == "Q") {
        key = KeyCode::Q;
    } else if (src == "R") {
        key = KeyCode::R;
    } else if (src == "S") {
        key = KeyCode::S;
    } else if (src == "T") {
        key = KeyCode::T;
    } else if (src == "U") {
        key = KeyCode::U;
    } else if (src == "V") {
        key = KeyCode::V;
    } else if (src == "W") {
        key = KeyCode::W;
    } else if (src == "X") {
        key = KeyCode::X;
    } else if (src == "Y") {
        key = KeyCode::Y;
    } else if (src == "Z") {
        key = KeyCode::Z;
    } else if (src == "Return") {
        key = KeyCode::Return;
    } else if (src == "Space") {
        key = KeyCode::Space;
    } else if (src == "LeftShift") {
        key = KeyCode::LeftShift;
    } else if (src == "RightShift") {
        key = KeyCode::RightShift;
    } else if (src == "LeftControl") {
        key = KeyCode::LeftControl;
    } else if (src == "RightControl") {
        key = KeyCode::RightControl;
    } else if (src == "LeftAlt") {
        key = KeyCode::LeftAlt;
    } else if (src == "RightAlt") {
        key = KeyCode::RightAlt;
    } else if (src == "Tab") {
        key = KeyCode::Tab;
    } else if (src == "Escape") {
        key = KeyCode::Escape;
    } else if (src == "UpArrow") {
        key = KeyCode::UpArrow;
    } else if (src == "DownArrow") {
        key = KeyCode::DownArrow;
    } else if (src == "LeftArrow") {
        key = KeyCode::LeftArrow;
    } else if (src == "RightArrow") {
        key = KeyCode::RightArrow;
    } else if (src == "Alpha1") {
        key = KeyCode::Alpha1;
    } else if (src == "Alpha2") {
        key = KeyCode::Alpha2;
    } else if (src == "Alpha3") {
        key = KeyCode::Alpha3;
    } else if (src == "Alpha4") {
        key = KeyCode::Alpha4;
    } else if (src == "Alpha5") {
        key = KeyCode::Alpha5;
    } else if (src == "Alpha6") {
        key = KeyCode::Alpha6;
    } else if (src == "Alpha7") {
        key = KeyCode::Alpha7;
    } else if (src == "Alpha8") {
        key = KeyCode::Alpha8;
    } else if (src == "Alpha9") {
        key = KeyCode::Alpha9;
    } else if (src == "Alpha0") {
        key = KeyCode::Alpha0;
    } else if (src == "F1") {
        key = KeyCode::F1;
    } else if (src == "F2") {
        key = KeyCode::F2;
    } else if (src == "F3") {
        key = KeyCode::F3;
    } else if (src == "F4") {
        key = KeyCode::F4;
    } else if (src == "F5") {
        key = KeyCode::F5;
    } else if (src == "F6") {
        key = KeyCode::F6;
    } else if (src == "F7") {
        key = KeyCode::F7;
    } else if (src == "F8") {
        key = KeyCode::F8;
    } else if (src == "F9") {
        key = KeyCode::F9;
    } else if (src == "F10") {
        key = KeyCode::F10;
    } else if (src == "F11") {
        key = KeyCode::F11;
    } else if (src == "F12") {
        key = KeyCode::F12;
    }

    if (key != KeyCode::None) {
        *dst = key;
    }
}
