#pragma once

#include "SystemInclude.h"
#include <rapidjson/document.h>
#include <memory>

class Window;
class FPSCounter;
class SceneManager;

class Game {
public:
    Game();
    ~Game();
    //アプリケーションの実行
    void run(HINSTANCE hInstance);
    //グローバルファイルを読み込む
    void loadProperties(const rapidjson::Value& inObj);

    //アプリケーションの終了
    static void quit();

private:
    //各クラスの初期化
    void initialize();
    //メインループ
    void mainLoop();

private:
    std::unique_ptr<Window> mWindow;
    std::unique_ptr<FPSCounter> mFPSCounter;
    SceneManager* mSceneManager;
    HINSTANCE mInstance;
    HWND mhWnd;
};
