#pragma once

#include "SystemInclude.h"
#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <string>

class Window {
public:
    Window();
    ~Window();
    void createWindow(HINSTANCE hInstance);
    LRESULT msgProc(HWND, UINT, WPARAM, LPARAM);
    HWND gethWnd() const;
    void update();
    static int width();
    static int height();
    static int standardWidth();
    static int standardHeight();
    static int debugWidth();
    static int debugHeight();
    static Vector2 windowToClientSize();
    //ゲームウィンドウをフルHDサイズに変える補正値を取得する
    static Vector2 getWindowCompensate();
    void loadProperties(const rapidjson::Value& inObj);

private:
    void updateWindowToClientSize();

private:
    HWND mhWnd;
    WNDCLASSEX mWndClass; //ウィンドウクラスの設定
    std::string mTitle;
    static inline int mWidth = 1080;
    static inline int mHeight = 720;
    static inline int mStandardWidth = 1920;
    static inline int mStandardHeight = 1080;
    static inline int mDebugWidth = 1920;
    static inline int mDebugHeight = 1080;
    static inline Vector2 mWindowToClientSize = Vector2::zero;
};

