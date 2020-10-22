#include "LevelLoader.h"
#include "FileUtil.h"
#include "../DebugLayer/Debug.h"
#include "../Device/AssetsManager.h"
#include "../GameObject/GameObject.h"
#include "../System/Game.h"
#include "../System/World.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <fstream>

bool LevelLoader::loadJSON(const std::string & filePath, rapidjson::Document * outDoc) {
    //フォルダ階層の移動
    World::instance().assetsManager().setDataDirectory(filePath);

    //ファイル名の取得
    auto fileName = FileUtil::getFileNameFromDirectry(filePath);

    //バイナリモードで開き、末尾に移動
    std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        Debug::windowMessage(filePath + "ファイルが見つかりません");
        return false;
    }

    //ファイルの末尾 = ファイルサイズ
    std::ifstream::pos_type fileSize = file.tellg();
    //ファイルの先頭まで戻る
    file.seekg(0, std::ios::beg);

    //末尾のヌルを含めたsize+1の配列を作る
    std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
    //配列にバイト列を書き込む
    file.read(bytes.data(), static_cast<size_t>(fileSize));

    //生データをRapidJSONドキュメントにロードする
    outDoc->Parse(bytes.data());
    if (!outDoc->IsObject()) {
        Debug::windowMessage(filePath + "ファイルは有効ではありません");
        return false;
    }

    return true;
}

void LevelLoader::loadGlobal(Game * root, const std::string & filePath) {
    rapidjson::Document doc;
    if (!loadJSON(filePath, &doc)) {
        Debug::windowMessage(filePath + ": レベルファイルのロードに失敗しました");
    }

    const rapidjson::Value& globals = doc["globalProperties"];
    if (!globals.IsObject()) {
        Debug::windowMessage(filePath + ": [globalProperties]が見つからないか、正しいオブジェクトではありません");
    }

    root->loadProperties(globals);
}

void LevelLoader::saveLevel(const std::string & filePath) {
    ////ドキュメントとルートオブジェクトを生成
    //rapidjson::Document doc;
    //doc.SetObject();

    //// Write the version
    ////JsonHelper::AddInt(doc.GetAllocator(), doc, "version", LevelVersion);

    ////グローバル
    //rapidjson::Value globals(rapidjson::kObjectType);
    //saveGlobalProperties(doc.GetAllocator(), renderer, &globals);
    //doc.AddMember("globalProperties", globals, doc.GetAllocator());

    ////アクター
    //rapidjson::Value actors(rapidjson::kArrayType);
    //saveActors(doc.GetAllocator(), renderer, &actors);
    //doc.AddMember("actors", actors, doc.GetAllocator());

    ////jsonを文字列バッファに保存
    //rapidjson::StringBuffer buffer;
    ////整形出力用にPrettyWriterを使う(もしくはWriter)
    //rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    //doc.Accept(writer);
    //const char* output = buffer.GetString();

    ////文字列をファイルに書き込む
    //std::ofstream outFile(filePath);
    //if (outFile.is_open()) {
    //    outFile << output;
    //}
}

void LevelLoader::saveUI(std::list<std::shared_ptr<GameObject>> uiList, const std::string & filePath) {
    //ドキュメントとルートオブジェクトを生成
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Value uis(rapidjson::kArrayType);
    for (const auto& ui : uiList) {
        //UI用のjsonオブジェクトを作る
        rapidjson::Value obj(rapidjson::kObjectType);
        //タイプを追加
        JsonHelper::setString(doc.GetAllocator(), &obj, "type", ui->tag());

        //プロパティ用のjsonオブジェクトを作る
        rapidjson::Value props(rapidjson::kObjectType);
        //プロパティを保存
        ui->saveProperties(doc.GetAllocator(), &props);
        //プロパティをUIのjsonオブジェクトに追加
        obj.AddMember("properties", props, doc.GetAllocator());

        //コンポーネントを保存
        //rapidjson::Value components(rapidjson::kArrayType);
        //saveComponents(alloc, actor, &components);
        //obj.AddMember("components", components, alloc);

        //UIを配列に追加
        uis.PushBack(obj, doc.GetAllocator());
    }
    doc.AddMember("UIs", uis, doc.GetAllocator());

    //jsonを文字列バッファに保存
    rapidjson::StringBuffer buffer;
    //整形出力用にPrettyWriterを使う(もしくはWriter)
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* output = buffer.GetString();

    //文字列をファイルに書き込む
    World::instance().assetsManager().setDataDirectory(filePath);
    auto fileName = FileUtil::getFileNameFromDirectry(filePath);
    std::ofstream outFile(fileName);
    if (outFile.is_open()) {
        outFile << output;
    }
}

//void LevelLoader::saveActors(rapidjson::Document::AllocatorType & alloc, ActorManager * manager, rapidjson::Value * inArray) const {
//    //for (const auto& actor : manager->getActors()) {
//    //    //アクター用のjsonオブジェクトを作る
//    //    rapidjson::Value obj(rapidjson::kObjectType);
//    //    //タイプを追加
//    //    JsonHelper::setString(alloc, &obj, "type", actor->tag());
//
//    //    //プロパティ用のjsonオブジェクトを作る
//    //    rapidjson::Value props(rapidjson::kObjectType);
//    //    //プロパティを保存
//    //    actor->saveProperties(alloc, &props);
//    //    //プロパティをアクターのjsonオブジェクトに追加
//    //    obj.AddMember("properties", props, alloc);
//
//    //    //コンポーネントを保存
//    //    rapidjson::Value components(rapidjson::kArrayType);
//    //    saveComponents(alloc, actor, &components);
//    //    obj.AddMember("components", components, alloc);
//
//    //    //アクターを配列に追加
//    //    inArray->PushBack(obj, alloc);
//    //}
//}

//void LevelLoader::saveComponents(rapidjson::Document::AllocatorType & alloc, const std::shared_ptr<Actor> actor, rapidjson::Value * inArray) const {
//    //const auto& components = actor->getComponents();
//    //for (const Component& comp : components) {
//    //    // Make a JSON object
//    //    rapidjson::Value obj(rapidjson::kObjectType);
//    //    // Add type
//    //    JsonHelper::setString(alloc, &obj, "type", comp->get());
//
//    //    // Make an object for properties
//    //    rapidjson::Value props(rapidjson::kObjectType);
//    //    // Save rest of properties
//    //    comp->SaveProperties(alloc, props);
//    //    // Add the member
//    //    obj.AddMember("properties", props, alloc);
//
//    //    // Add component to array
//    //    inArray.PushBack(obj, alloc);
//    //}
//}

bool JsonHelper::getInt(const rapidjson::Value & inObject, const char* inProperty, int* out) {
    //プロパティが存在するか
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    //値の方を取得し、整数であることを確認
    auto& property = itr->value;
    if (!property.IsInt()) {
        return false;
    }

    //プロパティの取得に成功
    *out = property.GetInt();
    return true;
}

bool JsonHelper::getFloat(const rapidjson::Value & inObject, const char* inProperty, float* out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsDouble()) {
        return false;
    }

    *out = static_cast<float>(property.GetDouble());
    return true;
}

bool JsonHelper::getString(const rapidjson::Value & inObject, const char* inProperty, std::string * out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsString()) {
        return false;
    }

    *out = property.GetString();
    return true;
}

bool JsonHelper::getBool(const rapidjson::Value & inObject, const char* inProperty, bool* out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsBool()) {
        return false;
    }

    *out = property.GetBool();
    return true;
}

bool JsonHelper::getVector2(const rapidjson::Value & inObject, const char* inProperty, Vector2 * out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsArray() || property.Size() != 2) {
        return false;
    }

    for (rapidjson::SizeType i = 0; i < 2; i++) {
        if (!property[i].IsDouble()) {
            return false;
        }
    }

    out->x = static_cast<float>(property[0].GetDouble());
    out->y = static_cast<float>(property[1].GetDouble());

    return true;
}

bool JsonHelper::getVector3(const rapidjson::Value & inObject, const char* inProperty, Vector3 * out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsArray() || property.Size() != 3) {
        return false;
    }

    for (rapidjson::SizeType i = 0; i < 3; i++) {
        if (!property[i].IsDouble()) {
            return false;
        }
    }

    out->x = static_cast<float>(property[0].GetDouble());
    out->y = static_cast<float>(property[1].GetDouble());
    out->z = static_cast<float>(property[2].GetDouble());

    return true;
}

bool JsonHelper::getVector4(const rapidjson::Value& inObject, const char* inProperty, Vector4* out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsArray() || property.Size() != 4) {
        return false;
    }

    for (rapidjson::SizeType i = 0; i < 4; i++) {
        if (!property[i].IsDouble()) {
            return false;
        }
    }

    out->x = static_cast<float>(property[0].GetDouble());
    out->y = static_cast<float>(property[1].GetDouble());
    out->z = static_cast<float>(property[2].GetDouble());
    out->w = static_cast<float>(property[3].GetDouble());

    return true;
}

bool JsonHelper::getQuaternion(const rapidjson::Value & inObject, const char* inProperty, Quaternion * out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsArray() || property.Size() != 4) {
        return false;
    }

    for (rapidjson::SizeType i = 0; i < 4; i++) {
        if (!property[i].IsDouble()) {
            return false;
        }
    }

    out->x = static_cast<float>(property[0].GetDouble());
    out->y = static_cast<float>(property[1].GetDouble());
    out->z = static_cast<float>(property[2].GetDouble());
    out->w = static_cast<float>(property[3].GetDouble());

    return true;
}

bool JsonHelper::getStringArray(const rapidjson::Value& inObject, const char* inProperty, std::vector<std::string>* out) {
    auto itr = inObject.FindMember(inProperty);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if (!property.IsArray()) {
        return false;
    }

    std::vector<std::string> temp;
    for (rapidjson::SizeType i = 0; i < property.Size(); i++) {
        if (!property[i].IsString()) {
            return false;
        }
        temp.emplace_back(property[i].GetString());
    }

    *out = temp;

    return true;
}

void JsonHelper::setInt(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, int value) {
    rapidjson::Value v(value);
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setFloat(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, float value) {
    rapidjson::Value v(value);
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setString(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, const std::string & value) {
    rapidjson::Value v;
    v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setBool(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, bool value) {
    rapidjson::Value v(value);
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setVector2(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, const Vector2 & value) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);
    //x, y, zそれぞれ追加
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);

    //inObjectに配列として追加
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setVector3(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, const Vector3 & value) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);
    //x, y, zそれぞれ追加
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);

    //inObjectに配列として追加
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setVector4(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, const Vector4& value) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);
    //x, y, z, wそれぞれ追加
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);
    v.PushBack(rapidjson::Value(value.w).Move(), alloc);

    //inObjectに配列として追加
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setQuaternion(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObject, const char* name, const Quaternion & value) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);
    //x, y, z, wそれぞれ追加
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);
    v.PushBack(rapidjson::Value(value.w).Move(), alloc);

    //inObjectに配列として追加
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}
