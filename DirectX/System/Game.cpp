#include "Game.h"
#include "AssetsManager.h"
#include "GlobalFunction.h"
#include "SceneManager.h"
#include "Window.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "../DebugLayer/DebugUtility.h"
#include "../Device/FPSCounter.h"
#include "../DirectX/DirectX.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Input/InputUtility.h"
#include "../Sound/XAudio2/SoundEngine.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/Random.h"

Game::Game() :
    mWindow(nullptr),
    mFPSCounter(nullptr),
    mSceneManager(nullptr),
    mInstance(nullptr) {
}

Game::~Game() {
    safeDelete(mSceneManager);

    //imguiの終了処理
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    Shader::finalize();
    Texture::finalize();
    GameObjectCreater::finalize();
    InputUtility::finalize();
    DebugUtility::finalize();
    SoundEngine::instance().finalize();
    AssetsManager::instance().finalize();
    MyDirectX::DirectX::instance().finalize();
}

void Game::run(HINSTANCE hInstance) {
    mInstance = hInstance;
    initialize();

    MSG msg = { 0 };
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            mainLoop();
        }
    }
}

void Game::loadProperties(const rapidjson::Value& inObj) {
    mWindow->loadProperties(inObj);
    mFPSCounter->loadProperties(inObj);
    DebugUtility::loadProperties(inObj);
    InputUtility::loadProperties(inObj);
    mSceneManager->loadProperties(inObj);
}

void Game::quit() {
    PostQuitMessage(0);
}

void Game::initialize() {
    mWindow = std::make_unique<Window>();

    mFPSCounter = std::make_unique<FPSCounter>();
    DebugUtility::create();
    InputUtility::create();
    mSceneManager = new SceneManager();

    //ファイルから値を読み込む
    LevelLoader::loadGlobal(this, "Global.json");

    mWindow->createWindow(mInstance);
    const auto& hwnd = mWindow->gethWnd();

    MyDirectX::DirectX::instance().initialize(hwnd);

    //imguiの初期化
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(MyDirectX::DirectX::instance().device(), MyDirectX::DirectX::instance().deviceContext());

    Random::initialize();
    DebugUtility::initialize();
    InputUtility::initialize(hwnd);
    GameObjectCreater::initialize();
    mSceneManager->initialize();
}

void Game::mainLoop() {
    //imguiの描画前準備
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    auto& dx = MyDirectX::DirectX::instance();
    dx.clearRenderTarget();
    dx.clearDepthStencilView();

    InputUtility::update();
    mWindow->update();

    mSceneManager->update();
    mSceneManager->draw();

    //imguiの描画
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    mFPSCounter->fixedFrame();
    dx.present();

    SoundEngine::instance().update();
}
