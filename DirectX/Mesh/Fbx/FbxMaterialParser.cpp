#include "FbxMaterialParser.h"
#include "../../System/AssetsManager.h"
#include "../../System/Texture/TextureFromFile.h"
#include "../../Utility/FileUtil.h"

FbxMaterialParser::FbxMaterialParser() = default;

FbxMaterialParser::~FbxMaterialParser() = default;

void FbxMaterialParser::parse(Material& material, FbxMesh* fbxMesh, const std::string& directoryPath) const {
    //マテリアルがなければ終了
    if (fbxMesh->GetElementMaterialCount() == 0) {
        return;
    }

    //Mesh側のマテリアル情報を取得
    FbxLayerElementMaterial* fbxMaterial = fbxMesh->GetElementMaterial(0);

    //インデックスバッファからインデックスを取得
    int index = fbxMaterial->GetIndexArray().GetAt(0);
    //マテリアル取得
    FbxSurfaceMaterial* surfaceMaterial = fbxMesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

    //マテリアル名の保存
    if (surfaceMaterial) {
        //マテリアル名を取得
        material.materialName = surfaceMaterial->GetName();

        //テクスチャ以外の情報を取得
        loadMaterialAttribute(material, surfaceMaterial);

        //テクスチャを取得
        loadTextures(material, surfaceMaterial, directoryPath);
    }
}

void FbxMaterialParser::loadMaterialAttribute(Material& material, FbxSurfaceMaterial* fbxSurfaceMaterial) const {
    //継承関係にあるか調べる
    if (!fbxSurfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
        return;
    }

    FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(fbxSurfaceMaterial);
    if (!lambert) {
        return;
    }

    //まずはランバート分読み込む
    loadLambert(material, lambert);

    //継承関係にあるか調べる
    if (lambert->GetClassId().Is(FbxSurfacePhong::ClassId)) {
        return;
    }

    FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(lambert);
    if (!phong) {
        return;
    }

    //Phongだったら追加読み込み
    loadPhong(material, phong);
}

void FbxMaterialParser::loadLambert(Material& material, const FbxSurfaceLambert* fbxSurfaceLambert) const {
    //アンビエント
    auto prop = fbxSurfaceLambert->FindProperty(FbxSurfaceMaterial::sAmbient);
    if (prop.IsValid()) {
        const auto& ambient = fbxSurfaceLambert->Ambient.Get();
        material.ambient.x = static_cast<float>(ambient[0]);
        material.ambient.y = static_cast<float>(ambient[1]);
        material.ambient.z = static_cast<float>(ambient[2]);
    }

    //ディヒューズ
    prop = fbxSurfaceLambert->FindProperty(FbxSurfaceMaterial::sDiffuse);
    if (prop.IsValid()) {
        const auto& diffuse = fbxSurfaceLambert->Diffuse.Get();
        material.diffuse.x = static_cast<float>(diffuse[0]);
        material.diffuse.y = static_cast<float>(diffuse[1]);
        material.diffuse.z = static_cast<float>(diffuse[2]);
    }

    //エミッシブ
    prop = fbxSurfaceLambert->FindProperty(FbxSurfaceMaterial::sEmissive);
    if (prop.IsValid()) {
        const auto& emissive = fbxSurfaceLambert->Emissive.Get();
        material.emissive.x = static_cast<float>(emissive[0]);
        material.emissive.y = static_cast<float>(emissive[1]);
        material.emissive.z = static_cast<float>(emissive[2]);
    }

    //バンプ
    prop = fbxSurfaceLambert->FindProperty(FbxSurfaceMaterial::sBump);
    if (prop.IsValid()) {
        const auto& bump = fbxSurfaceLambert->Bump.Get();
        material.bump.x = static_cast<float>(bump[0]);
        material.bump.y = static_cast<float>(bump[1]);
        material.bump.z = static_cast<float>(bump[2]);
    }

    //透過度
    prop = fbxSurfaceLambert->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
    if (prop.IsValid()) {
        material.transparency = static_cast<float>(fbxSurfaceLambert->TransparencyFactor.Get());
    }
}

void FbxMaterialParser::loadPhong(Material& material, const FbxSurfacePhong* fbxSurfacePhong) const {
    //スペキュラ
    auto prop = fbxSurfacePhong->FindProperty(FbxSurfaceMaterial::sSpecular);
    if (prop.IsValid()) {
        const auto& specular = fbxSurfacePhong->Specular.Get();
        material.specular.x = static_cast<float>(specular[0]);
        material.specular.y = static_cast<float>(specular[1]);
        material.specular.z = static_cast<float>(specular[2]);
    }

    //光沢
    prop = fbxSurfacePhong->FindProperty(FbxSurfaceMaterial::sShininess);
    if (prop.IsValid()) {
        material.shininess = static_cast<float>(fbxSurfacePhong->Shininess.Get());
    }
}

void FbxMaterialParser::loadTextures(Material& material, FbxSurfaceMaterial* fbxSurfaceMaterial, const std::string& directoryPath) const {
    //テクスチャ作成
    createTexture(material, fbxSurfaceMaterial, directoryPath, FbxSurfaceMaterial::sDiffuse);
    createTexture(material, fbxSurfaceMaterial, directoryPath, FbxSurfaceMaterial::sNormalMap);
}

void FbxMaterialParser::createTexture(Material& material, const FbxSurfaceMaterial* fbxSurfaceMaterial, const std::string& directoryPath, const char* type) const {
    //プロパティを取得する
    const auto& prop = fbxSurfaceMaterial->FindProperty(type);

    //テクスチャを取得する
    auto fbxTexture = getFbxTexture(prop);

    //テクスチャを使用していなかったら終了
    if (!fbxTexture) {
        return;
    }

    //ファイルパスを相対パスで取得
    auto filePath = fbxTexture->GetRelativeFileName();
    //ファイルパスからファイル名を取得する
    const auto& textureName = FileUtil::getFileNameFromDirectry(filePath);

    //ファイル名からテクスチャを作成する
    const auto& tex = AssetsManager::instance().createTexture(textureName, directoryPath);

    //指定されたテクスチャに渡す
    if (type == FbxSurfaceMaterial::sDiffuse) {
        material.texture = tex;
    } else if (type == FbxSurfaceMaterial::sNormalMap) {
        material.normalMapTexture = tex;
    }
}

FbxFileTexture* FbxMaterialParser::getFbxTexture(const FbxProperty& prop) const {
    FbxFileTexture* out = nullptr;

    int numTexture = prop.GetSrcObjectCount<FbxFileTexture>();
    if (numTexture > 0) {
        out = prop.GetSrcObject<FbxFileTexture>(0);
    } else {
        //失敗したらマルチテクスチャの可能性を考えて、FbxLayerdTextureを指定する
        //FbxLayerdTextureからテクスチャを取得する
        int numLayer = prop.GetSrcObjectCount<FbxLayeredTexture>();
        if (numLayer > 0) {
            out = prop.GetSrcObject<FbxFileTexture>(0);
        }
    }

    return out;
}
