#include "GameObjectFactory.h"
#include "GameObject.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Camera/CameraMove.h"
#include "../Component/CharacterOperation/CharacterCost.h"
#include "../Component/CharacterOperation/CharacterCreater.h"
#include "../Component/CharacterOperation/CostRenderer.h"
#include "../Component/CharacterOperation/DragAndDropCharacter.h"
#include "../Component/CollideOperation/AABBMouseScaler.h"
#include "../Component/CollideOperation/AABBSelector.h"
#include "../Component/CollideOperation/CollideAdder.h"
#include "../Component/CollideOperation/CollideMouseOperator.h"
#include "../Component/CollideOperation/MeshAdder.h"
#include "../Component/Collider/AABBCollider.h"
#include "../Component/Collider/CircleCollider.h"
#include "../Component/Collider/SphereCollider.h"
#include "../Component/Light/DirectionalLight.h"
#include "../Component/Light/PointLightComponent.h"
#include "../Component/Mesh/MeshComponent.h"
#include "../Component/Mesh/SkinMeshComponent.h"
#include "../Component/Other/HitPointComponent.h"
#include "../Component/Sample/RayMouse.h"
#include "../Component/Scene/GamePlay.h"
#include "../Component/Scene/Scene.h"
#include "../Component/Scene/Title.h"
#include "../Component/Sound/ListenerComponent.h"
#include "../Component/Sound/SoundComponent.h"
#include "../Component/Sound/WaveformRenderSample.h"
#include "../Component/Sprite/Sprite3D.h"
#include "../Component/Sprite/SpriteComponent.h"
#include "../Component/Text/Text.h"
#include "../Component/Text/TextFloat.h"
#include "../Component/Text/TextNumber.h"
#include "../DebugLayer/Debug.h"
#include "../System/GlobalFunction.h"
#include "../Utility/LevelLoader.h"
#include <cassert>

//練習がてら、ちょい楽できるように
#define ADD_COMPONENT(className) { mComponents.emplace((#className), &Component::create<className>); }

GameObjectFactory::GameObjectFactory() {
    assert(!mInstantiated);
    mInstantiated = true;

    ADD_COMPONENT(Camera);
    ADD_COMPONENT(CameraMove);

    ADD_COMPONENT(CharacterCost);
    ADD_COMPONENT(CharacterCreater);
    ADD_COMPONENT(CostRenderer);
    ADD_COMPONENT(DragAndDropCharacter);

    ADD_COMPONENT(AABBMouseScaler);
    ADD_COMPONENT(AABBSelector);
    ADD_COMPONENT(CollideAdder);
    ADD_COMPONENT(CollideMouseOperator);
    ADD_COMPONENT(MeshAdder);

    ADD_COMPONENT(AABBCollider);
    ADD_COMPONENT(CircleCollider);
    ADD_COMPONENT(SphereCollider);

    ADD_COMPONENT(DirectionalLight);
    ADD_COMPONENT(PointLightComponent);

    ADD_COMPONENT(MeshComponent);
    ADD_COMPONENT(SkinMeshComponent);

    ADD_COMPONENT(HitPointComponent);

    ADD_COMPONENT(RayMouse);

    ADD_COMPONENT(GamePlay);
    ADD_COMPONENT(Scene);
    ADD_COMPONENT(Title);

    ADD_COMPONENT(ListenerComponent);
    ADD_COMPONENT(SoundComponent);
    ADD_COMPONENT(WaveformRenderSample);

    ADD_COMPONENT(Sprite3D);
    ADD_COMPONENT(SpriteComponent);

    ADD_COMPONENT(Text);
    ADD_COMPONENT(TextFloat);
    ADD_COMPONENT(TextNumber);
}

GameObjectFactory::~GameObjectFactory() {
    mInstantiated = false;
}

std::shared_ptr<GameObject> GameObjectFactory::createGameObjectFromFile(const std::string& type, const std::string& directoryPath) {
    //ディレクトパスとタイプからファイルパスを作成
    auto filePath = directoryPath + type + ".json";

    rapidjson::Document document;
    if (!LevelLoader::loadJSON(filePath, &document)) {
        Debug::windowMessage(filePath + ": レベルファイルのロードに失敗しました");
        return nullptr;
    }

    return createGameObject(document, type);
}

std::shared_ptr<GameObject> GameObjectFactory::createGameObject(const rapidjson::Document& inDocument, const std::string& type) {
    //タグを読み込む
    const auto& tag = loadTag(inDocument);
    //ゲームオブジェクトを生成
    auto gameObject = GameObject::create(type, tag);
    //プロパティを読み込む
    loadGameObjectProperties(*gameObject, inDocument);

    //コンポーネントがあれば取得
    loadComponents(*gameObject, inDocument);

    return gameObject;
}

std::string GameObjectFactory::loadTag(const rapidjson::Document& inDocument) {
    //初期タグをNoneにする
    std::string tag = "None";
    //タグ属性があれば読み込む
    JsonHelper::getString(inDocument, "tag", &tag);

    return tag;
}

void GameObjectFactory::loadGameObjectProperties(GameObject& gameObject, const rapidjson::Document& inDocument) {
    if (inDocument.HasMember("properties")) {
        gameObject.loadProperties(inDocument["properties"]);
    }
}

void GameObjectFactory::loadComponents(GameObject& gameObject, const rapidjson::Document& inDocument) const {
    //ファイルにcomponentsメンバがなければ終了
    if (!inDocument.HasMember("components")) {
        return;
    }

    const auto& components = inDocument["components"];
    //componentsメンバが配列じゃなければなければ終了
    if (!components.IsArray()) {
        return;
    }

    for (rapidjson::SizeType i = 0; i < components.Size(); ++i) {
        //各コンポーネントを読み込んでいく
        loadComponent(gameObject, components[i]);
    }
}

void GameObjectFactory::loadComponent(GameObject& gameObject, const rapidjson::Value& component) const {
    //有効なオブジェクトか
    if (!component.IsObject()) {
        return;
    }
    //有効な型名か
    std::string type;
    if (!isValidType(type, component)) {
        return;
    }
    //mapに存在するか
    auto itr = mComponents.find(type);
    if (itr == mComponents.end()) {
        Debug::windowMessage(type + "は有効な型ではありません");
        return;
    }
    //新規コンポーネントを生成
    itr->second(gameObject, type, component["properties"]);
}

bool GameObjectFactory::isValidType(std::string& outType, const rapidjson::Value& inObj) const {
    return (JsonHelper::getString(inObj, "type", &outType));
}



void GameObjectCreater::initialize() {
    mFactory = new GameObjectFactory();
}

void GameObjectCreater::finalize() {
    safeDelete(mFactory);
}

std::shared_ptr<GameObject> GameObjectCreater::create(const std::string& type) {
    return mFactory->createGameObjectFromFile(type);
}
