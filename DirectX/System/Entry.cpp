#include "Game.h"
#include "World.h"
#include <windows.h>
//メモリリーク検出用
#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#endif // _DEBUG
//#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT) {
    Game* game = new Game();
    if (!game) {
        MessageBox(0, L"クラス生成失敗 アプリを終了します", NULL, MB_OK);
        return 0;
    }

    game->run(hInstance);

    delete game;
    game = nullptr;

    World::instance().finalize();

    //メモリリーク検出関数
#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
#endif // _DEBUG

    return 0;
}