#include "Game.h"
#include "GlobalFunction.h"
#include "SceneManager.h"
#include "Window.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "../DebugLayer/DebugUtility.h"
#include "../Device/FPSCounter.h"
#include "../DirectX/DirectX.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Input/InputUtility.h"
#include "../Sound/XAudio2/SoundEngine.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/Random.h"

Game::Game() :
    mWindow(nullptr),
    mFPSCounter(nullptr),
    mSceneManager(nullptr),
    mInstance(nullptr),
    mhWnd(nullptr) {
}

Game::~Game() {
    safeDelete(mSceneManager);

    Shader::finalize();
    Texture::finalize();
    GameObjectCreater::finalize();
    InputUtility::finalize();
    DebugUtility::finalize();
    SoundEngine::instance().finalize();
    DirectX::instance().finalize();
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
    mhWnd = mWindow->gethWnd();

    DirectX::instance().initialize(mhWnd);

    Random::initialize();
    DebugUtility::initialize();
    InputUtility::initialize(mhWnd);
    GameObjectCreater::initialize();
    mSceneManager->initialize();
}

void Game::mainLoop() {
    auto& dx = DirectX::instance();
    dx.clearRenderTarget();
    dx.clearDepthStencilView();

    InputUtility::update();
    mWindow->update();

    mSceneManager->update();
    mSceneManager->draw();

    mFPSCounter->fixedFrame();
    dx.present();

    SoundEngine::instance().update();
}
