#pragma once

#include "../System/SystemInclude.h"
#include <rapidjson/document.h>
#include <dinput.h>
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dinput8.lib")

class Keyboard;
class Mouse;
class JoyPad;

class InputUtility {
public:
    //各種入力クラス生成
    static void create();
    //各種入力クラス初期化
    static bool initialize(const HWND& hWnd);
    //ファイルから読み取り
    static void loadProperties(const rapidjson::Value& inObj);
    //終了処理
    static void finalize();
    //毎フレーム更新
    static void update();

    //各種入力クラスにアクセス
    static Keyboard& keyboard();
    static Mouse& mouse();
    static JoyPad& joyPad();

private:
    //生成、コピー禁止
    InputUtility() = delete;
    ~InputUtility() = delete;
    InputUtility(const InputUtility&) = delete;
    InputUtility& operator=(const InputUtility&) = delete;

public:
    static inline IDirectInput8* mDirectInput = nullptr;

private:
    static inline Keyboard* mKeyboard = nullptr;
    static inline Mouse* mMouse = nullptr;
    static inline JoyPad* mJoyPad = nullptr;
};
