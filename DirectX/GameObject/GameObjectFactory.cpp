#include "GameObjectFactory.h"
#include "GameObject.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Camera/LookAtObject.h"
#include "../Component/Collider/CircleCollider.h"
#include "../Component/Collider/SphereCollider.h"
#include "../Component/Light/DirectionalLight.h"
#include "../Component/Light/PointLightComponent.h"
#include "../Component/Mesh/MeshComponent.h"
#include "../Component/Other/Fade.h"
#include "../Component/Other/HitPointComponent.h"
#include "../Component/Player/PlayerMove.h"
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
    ADD_COMPONENT(LookAtObject);

    ADD_COMPONENT(CircleCollider);
    ADD_COMPONENT(SphereCollider);

    ADD_COMPONENT(DirectionalLight);
    ADD_COMPONENT(PointLightComponent);

    ADD_COMPONENT(MeshComponent);

    ADD_COMPONENT(Fade);
    ADD_COMPONENT(HitPointComponent);

    ADD_COMPONENT(PlayerMove);

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

    const std::string& fileName = "ActorsList.json";
    if (!LevelLoader::loadJSON(fileName, &mDocument)) {
        Debug::windowMessage(fileName + ": レベルファイルのロードに失敗しました");
    }
}

GameObjectFactory::~GameObjectFactory() {
    mInstantiated = false;
}

std::shared_ptr<GameObject> GameObjectFactory::loadGameObjectFromFile(const std::string& type) const {
    GameObjectPtr obj = nullptr;
    const auto& objs = mDocument["gameObjects"];
    if (objs.IsArray()) {
        obj = loadGameObjectProperties(objs, type);
    }

    return obj;
}

std::shared_ptr<GameObject> GameObjectFactory::loadGameObjectProperties(const rapidjson::Value& inArray, const std::string& type) const {
    GameObjectPtr gameObj = nullptr;
    //アクターの配列をループ
    for (rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        //有効なオブジェクトか
        const auto& obj = inArray[i];
        if (!obj.IsObject()) {
            continue;
        }
        //有効な型名か
        std::string t;
        if (!JsonHelper::getString(obj, "type", &t)) {
            continue;
        }
        //指定のタイプと一致するか
        if (t != type) {
            continue;
        }
        //mapからゲームオブジェクトを生成
        gameObj = GameObject::create();
        gameObj->setTag(type);
        if (obj.HasMember("properties")) {
            gameObj->loadProperties(obj["properties"]);
        }
        //コンポーネントプロパティがあれば取得
        if (!obj.HasMember("components")) {
            break;
        }
        const auto& components = obj["components"];
        if (components.IsArray()) {
            loadComponents(*gameObj, components);
            break;
        }
    }

    return gameObj;
}

void GameObjectFactory::loadComponents(GameObject& gameObject, const rapidjson::Value& inArray) const {
    //コンポーネントの配列をループ
    for (rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        //有効なオブジェクトか
        const auto& compObj = inArray[i];
        if (!compObj.IsObject()) {
            continue;
        }
        //有効な型名か
        std::string type;
        if (!JsonHelper::getString(compObj, "type", &type)) {
            continue;
        }
        //mapに存在するか
        auto itr = mComponents.find(type);
        if (itr == mComponents.end()) {
            Debug::windowMessage(type + "は有効な型ではありません");
            continue;
        }
        //新規コンポーネントを生成
        itr->second(gameObject, type, compObj["properties"]);
    }
}

void GameObjectCreater::initialize() {
    mFactory = new GameObjectFactory();
}

void GameObjectCreater::finalize() {
    safeDelete(mFactory);
}

std::shared_ptr<GameObject> GameObjectCreater::create(const std::string& type) {
    return mFactory->loadGameObjectFromFile(type);
}
