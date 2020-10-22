#include "Input.h"
#include "InputUtility.h"

IKeyboard& Input::keyboard() {
    return InputUtility::keyboard();
}

IMouse& Input::mouse() {
    return InputUtility::mouse();
}

JoyPad& Input::joyPad() {
    return InputUtility::joyPad();
}
