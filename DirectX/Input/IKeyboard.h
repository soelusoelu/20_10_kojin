#pragma once

#include <dinput.h>

enum class KeyCode {
    A = DIK_A,
    B = DIK_B,
    C = DIK_C,
    D = DIK_D,
    E = DIK_E,
    F = DIK_F,
    G = DIK_G,
    H = DIK_H,
    I = DIK_I,
    J = DIK_J,
    K = DIK_K,
    L = DIK_L,
    M = DIK_M,
    N = DIK_N,
    O = DIK_O,
    P = DIK_P,
    Q = DIK_Q,
    R = DIK_R,
    S = DIK_S,
    T = DIK_T,
    U = DIK_U,
    V = DIK_V,
    W = DIK_W,
    X = DIK_X,
    Y = DIK_Y,
    Z = DIK_Z,
    Return = DIK_RETURN,
    Space = DIK_SPACE,
    LeftShift = DIK_LSHIFT,
    RightShift = DIK_RSHIFT,
    LeftControl = DIK_LCONTROL,
    RightControl = DIK_RCONTROL,
    LeftAlt = DIK_LALT,
    RightAlt = DIK_RALT,
    Tab = DIK_TAB,
    Escape = DIK_ESCAPE,
    UpArrow = DIK_UPARROW,
    DownArrow = DIK_DOWNARROW,
    LeftArrow = DIK_LEFTARROW,
    RightArrow = DIK_RIGHTARROW,
    Alpha1 = DIK_1,
    Alpha2 = DIK_2,
    Alpha3 = DIK_3,
    Alpha4 = DIK_4,
    Alpha5 = DIK_5,
    Alpha6 = DIK_6,
    Alpha7 = DIK_7,
    Alpha8 = DIK_8,
    Alpha9 = DIK_9,
    Alpha0 = DIK_0,
    F1 = DIK_F1,
    F2 = DIK_F2,
    F3 = DIK_F3,
    F4 = DIK_F4,
    F5 = DIK_F5,
    F6 = DIK_F6,
    F7 = DIK_F7,
    F8 = DIK_F8,
    F9 = DIK_F9,
    F10 = DIK_F10,
    F11 = DIK_F11,
    F12 = DIK_F12,

    None = 512
};

class IKeyboard {
public:
    virtual ~IKeyboard() = default;
    //キーが押された瞬間
    virtual bool getKeyDown(KeyCode key) const = 0;
    //キーが押され続けているか
    virtual bool getKey(KeyCode key) const = 0;
    //キーが離れた瞬間
    virtual bool getKeyUp(KeyCode key) const = 0;
    //縦横
    virtual int horizontal() const = 0;
    virtual int vertical() const = 0;
    //決定キーが押された瞬間か
    virtual bool getEnter() const = 0;
};
