#include "LevelLoader.h"
#include "FileUtil.h"
#include "../Component/ComponentManager.h"
#include "../DebugLayer/Debug.h"
#include "../GameObject/GameObject.h"
#include "../System/Game.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <fstream>

bool LevelLoader::loadJSON(rapidjson::Document& outDoc, const std::string& fileName, const std::string& directoryPath) {
    const auto& filePath = directoryPath + fileName;

    //バイナリモードで開き、末尾に移動
    std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);
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
    outDoc.Parse(bytes.data());
    if (!outDoc.IsObject()) {
        Debug::windowMessage(filePath + "ファイルは有効ではありません");
        return false;
    }

    return true;
}

void LevelLoader::loadGlobal(Game* root, const std::string& filePath) {
    rapidjson::Document doc;
    if (!loadJSON(doc, filePath)) {
        return;
    }

    const rapidjson::Value& globals = doc["globalProperties"];
    if (!globals.IsObject()) {
        Debug::windowMessage(filePath + ": [globalProperties]が見つからないか、正しいオブジェクトではありません");
    }

    root->loadProperties(globals);
}

void LevelLoader::saveGameObject(const GameObject& gameObject, const std::string& directoryPath) {
    //ドキュメントとルートオブジェクトを生成
    rapidjson::Document doc;
    doc.SetObject();

    //アロケータの取得
    rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();

    //タグを追加
    JsonHelper::setString(alloc, &doc, "tag", gameObject.tag());

    //トランスフォーム用のjsonオブジェクトを作る
    rapidjson::Value props(rapidjson::kObjectType);
    //トランスフォームを保存
    gameObject.saveProperties(alloc, &props);
    //トランスフォームをゲームオブジェクトのjsonオブジェクトに追加
    doc.AddMember("transform", props, alloc);

    //コンポーネントを保存
    rapidjson::Value components(rapidjson::kArrayType);
    gameObject.componentManager().saveComponents(alloc, &components);
    doc.AddMember("components", components, alloc);


    //jsonを文字列バッファに保存
    rapidjson::StringBuffer buffer;
    //整形出力用にPrettyWriterを使う(もしくはWriter)
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* output = buffer.GetString();

    //文字列をファイルに書き込む
    std::ofstream outFile(directoryPath + gameObject.name() + ".json");
    if (outFile.is_open()) {
        outFile << output;
    }
}



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

void JsonHelper::setStringArray(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObject, const char* name, const std::vector<std::string>& values) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);

    //配列要素を順に追加していく
    rapidjson::Value temp;
    for (const auto& value : values) {
        temp.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);
        v.PushBack(temp, alloc);
    }

    //inObjectに配列として追加
    inObject->AddMember(rapidjson::StringRef(name), v, alloc);
}
