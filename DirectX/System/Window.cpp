#include "Window.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

Window* gWindow = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return gWindow->msgProc(hWnd, uMsg, wParam, lParam);
}

Window::Window() :
    mhWnd(nullptr),
    mWndClass(),
    mTitle("") {
}

Window::~Window() {
    //ウィンドウクラスを登録解除
    UnregisterClass(mWndClass.lpszClassName, mWndClass.hInstance);
}

void Window::createWindow(HINSTANCE hInstance) {
    gWindow = this;

    // ウィンドウの定義
    mWndClass.cbSize = sizeof(WNDCLASSEX);
    //mWndClass.style = CS_HREDRAW | CS_VREDRAW;
    mWndClass.lpfnWndProc = WndProc;
    mWndClass.hInstance = hInstance;
    //mWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    mWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    //mWndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    mWndClass.lpszClassName = StringUtil::charToWchar(mTitle.c_str());
    //mWndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    //ウィンドウクラスをOSに登録
    RegisterClassEx(&mWndClass);

    //ウィンドウサイズ { X 座標 Y 座標 横幅 縦幅 }
    RECT wrc{};
#ifdef _DEBUG
    wrc = { 0, 0, mDebugWidth, mDebugHeight };
#else
    wrc = { 0, 0, mWidth, mHeight };
#endif // _DEBUG
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); //自動でサイズ補正

    //ウィンドウの作成
    mhWnd = CreateWindow(
        mWndClass.lpszClassName,                        //クラス名
        StringUtil::charToWchar(mTitle.c_str()), //タイトルバーの文字
        WS_OVERLAPPEDWINDOW,                     //標準的なウィンドウスタイル
        CW_USEDEFAULT,                           //表示X座標(OSに任せる)
        CW_USEDEFAULT,                           //表示Y座標(OSに任せる)
        wrc.right - wrc.left,                    //ウィンドウ横幅
        wrc.bottom - wrc.top,                    //ウィンドウ縦幅
        nullptr,                                 //親ウィンドウハンドル
        nullptr,                                 //メニューハンドル
        hInstance,                               //呼び出しアプリケーションハンドル
        nullptr                                  //オプション
    );

    //ウインドウの表示
    ShowWindow(mhWnd, SW_SHOW);
    UpdateWindow(mhWnd);
}

LRESULT Window::msgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    switch (iMsg) {
    case WM_KEYDOWN:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

HWND Window::gethWnd() const {
    return mhWnd;
}

void Window::update() {
    updateWindowToClientSize();
}

int Window::width() {
    return mWidth;
}

int Window::height() {
    return mHeight;
}

int Window::standardWidth() {
    return mStandardWidth;
}

int Window::standardHeight() {
    return mStandardHeight;
}

int Window::debugWidth() {
    return mDebugWidth;
}

int Window::debugHeight() {
    return mDebugHeight;
}

Vector2 Window::windowToClientSize() {
    return mWindowToClientSize;
}

void Window::loadProperties(const rapidjson::Value& inObj) {
    const auto& windowObj = inObj["window"];
    if (windowObj.IsObject()) {
        JsonHelper::getString(windowObj, "title", &mTitle);
#ifdef _DEBUG
        JsonHelper::getInt(windowObj, "windowWidth", &mWidth);
        JsonHelper::getInt(windowObj, "windowHeight", &mHeight);
#else
        JsonHelper::getInt(windowObj, "releaseWindowWidth", &mWidth);
        JsonHelper::getInt(windowObj, "releaseWindowHeight", &mHeight);
#endif // _DEBUG
        JsonHelper::getInt(windowObj, "windowStandardWidth", &mStandardWidth);
        JsonHelper::getInt(windowObj, "windowStandardHeight", &mStandardHeight);
        JsonHelper::getInt(windowObj, "windowDebugWidth", &mDebugWidth);
        JsonHelper::getInt(windowObj, "windowDebugHeight", &mDebugHeight);
    }
}

void Window::updateWindowToClientSize() {
    RECT wRect, cRect; //ウィンドウ全体の矩形、クライアント領域の矩形
    int ww, wh;        //ウィンドウ全体の幅、高さ
    int cw, ch;        //クライアント領域の幅、高さ //ウィンドウ全体の幅・高さを計算

    //ウィンドウ全体の幅・高さを計算
    //GetWindowRect(mhWnd, &wRect);
    //ww = wRect.right - wRect.left;
    //wh = wRect.bottom - wRect.top;
    //クライアント領域の幅・高さを計算
    GetClientRect(mhWnd, &cRect);
    cw = cRect.right - cRect.left;
    ch = cRect.bottom - cRect.top;
    //ウィンドウとクライアントの比率を計算
#ifdef _DEBUG
    mWindowToClientSize.x = static_cast<float>(mDebugWidth) / static_cast<float>(cw);
    mWindowToClientSize.y = static_cast<float>(mDebugHeight) / static_cast<float>(ch);
#else
    mWindowToClientSize.x = static_cast<float>(mWidth) / static_cast<float>(cw);
    mWindowToClientSize.y = static_cast<float>(mHeight) / static_cast<float>(ch);
#endif // _DEBUG
}
