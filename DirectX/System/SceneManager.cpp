#include "SceneManager.h"
#include "Game.h"
#include "GlobalFunction.h"
#include "../Component/ComponentManager.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Scene/Scene.h"
#include "../Component/Text/TextBase.h"
#include "../DebugLayer/DebugUtility.h"
#include "../DebugLayer/LineRenderer/LineRenderer2D.h"
#include "../DebugLayer/LineRenderer/LineRenderer3D.h"
#include "../DebugLayer/Pause.h"
#include "../DebugLayer/PointRenderer.h"
#include "../Device/DrawString.h"
#include "../Device/Physics.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/GameObjectManager.h"
#include "../Input/Input.h"
#include "../Light/LightManager.h"
#include "../Mesh/MeshManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../Utility/LevelLoader.h"
#include <vector>

SceneManager::SceneManager() :
    mRenderer(std::make_unique<Renderer>()),
    mCurrentScene(nullptr),
    mCamera(nullptr),
    mGameObjectManager(std::make_unique<GameObjectManager>()),
    mMeshManager(std::make_unique<MeshManager>()),
    mSpriteManager(std::make_unique<SpriteManager>()),
    mPhysics(std::make_unique<Physics>()),
    mLightManager(std::make_unique<LightManager>()),
    mTextDrawer(new DrawString()),
    mBeginScene(),
    mShouldDraw(false) {
}

SceneManager::~SceneManager() {
    safeDelete(mTextDrawer);

    TextBase::setDrawString(nullptr);
}

void SceneManager::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "beginScene", &mBeginScene);
    mLightManager->loadProperties(inObj);
    mTextDrawer->loadProperties(inObj);

    std::vector<std::string> tags;
    JsonHelper::getStringArray(inObj, "removeExclusionTag", &tags);
    for (const auto& tag : tags) {
        mRemoveExclusionTags.emplace(tag);
    }
}

void SceneManager::initialize() {
    mRenderer->initialize();
    mLightManager->initialize();
    mTextDrawer->initialize();

    TextBase::setDrawString(mTextDrawer);

    auto cam = GameObjectCreater::create("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();

    mMeshManager->createShadowMap();

    mLightManager->createDirectionalLight();

    //初期シーンの設定
    createScene(mBeginScene);
}

void SceneManager::update() {
    //アップデートの最初で文字列削除
    DebugUtility::drawStringClear();
    mShouldDraw = true;

    //Escでゲーム終了
    if (Input::keyboard().getKeyDown(KeyCode::Escape)) {
        Game::quit();
    }

    //ポーズ中はデバッグだけアップデートを行う
    if (DebugUtility::pause().isPausing()) {
        DebugUtility::update();
        return;
    }

    //描画情報を削除
    DebugUtility::pointRenderer().clear();
    DebugUtility::lineRenderer2D().clear();
    DebugUtility::lineRenderer3D().clear();
    //保有しているテキストを全削除
    mTextDrawer->clear();
    //全ゲームオブジェクトの更新
    mGameObjectManager->update();
    //総当たり判定
    mPhysics->sweepAndPrune();
    //各マネージャークラスを更新
    mMeshManager->update();
    mSpriteManager->update();
    //デバッグ
    DebugUtility::update();

    //シーン移行
    const auto& next = mCurrentScene->getNext();
    if (!next.empty()) {
        change();
        //次のシーンに渡す値を避難させとく
        const auto& toNextValues = mCurrentScene->getValuePassToNextScene();
        //シーン遷移
        createScene(next);
        //新しいシーンに前のシーンの値を渡す
        mCurrentScene->getValueFromPreviousScene(toNextValues);
        //このフレームは描画しない
        mShouldDraw = false;
    }
}

void SceneManager::draw() const {
    if (!mShouldDraw) {
        return;
    }

    //各テクスチャ上にレンダリング
    //mRenderer->renderToTexture();
    //メッシュ描画準備
    //mRenderer->renderMesh();
    ////メッシュの一括描画
    //mMeshManager->draw(*mCamera, mLightManager->getDirectionalLight());
    //各テクスチャを参照してレンダリング
    //mRenderer->renderFromTexture(*mCamera, *mLightManager);
    ////ポイントライト描画準備
    //mRenderer->renderPointLight();
    ////ポイントライトの一括描画
    //mLightManager->drawPointLights(*mCamera);

    //メッシュ描画準備
    mRenderer->renderMesh();
    //メッシュの描画
    mMeshManager->draw(*mCamera, mLightManager->getDirectionalLight());

    //スプライト描画準備
    mRenderer->renderSprite();
    //3Dスプライト
    mRenderer->renderSprite3D();
    mSpriteManager->draw3Ds(mCamera->getView(), mCamera->getProjection());
    //2Dスプライト
    auto proj = Matrix4::identity;
    mRenderer->renderSprite2D(&proj);
    mSpriteManager->drawComponents(proj);

    //テキスト一括描画
    mTextDrawer->drawAll(proj);

#ifdef _DEBUG
    //レンダリング領域をデバッグに変更
    mRenderer->renderToDebug(&proj);
    //デバッグ表示
    DebugUtility::draw(proj);
#endif // _DEBUG

    //3Dポイント
    mRenderer->renderPoint3D();
    DebugUtility::pointRenderer().draw(mCamera->getViewProjection());
    //3Dライン
    mRenderer->renderLine3D();
    DebugUtility::lineRenderer3D().draw(mCamera->getViewProjection());
    //2Dライン
    mRenderer->renderLine2D(&proj);
    DebugUtility::lineRenderer2D().draw(proj);
}

void SceneManager::change() {
    mGameObjectManager->clear(mRemoveExclusionTags);
    mMeshManager->clear();
    mSpriteManager->clear();
}

void SceneManager::createScene(const std::string& name) {
    //シーン作成
    auto scene = GameObjectCreater::create(name);
    //シーンコンポーネント取得
    mCurrentScene = scene->componentManager().getComponent<Scene>();
}
