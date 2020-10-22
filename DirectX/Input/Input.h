#pragma once

#include "IKeyboard.h"
#include "IMouse.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "JoyPad.h"

class Input {
public:
    static IKeyboard& keyboard();
    static IMouse& mouse();
    static JoyPad& joyPad();

private:
    //生成、コピー禁止
    Input() = delete;
    ~Input() = delete;
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;
};
