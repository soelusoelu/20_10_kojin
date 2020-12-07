#pragma once

#include "../../Math/Math.h"
#include <fbxsdk.h>

//Fbxユーティリティ関数群
namespace FbxUtility {
//FbxMatirxからMatrix4へ変換する
Matrix4 substitutionMatrix(
    const FbxMatrix& src
);

//FbxDouble3からVector3へ変換する
Vector3 fbxDouble3ToVector3(
    const FbxDouble3& src
);

//FbxVector2からVector2へ変換する
Vector2 fbxVector2ToVector2(
    const FbxVector2& src
);

//FbxVector4からVector3へ変換する
Vector3 fbxVector4ToVector3(
    const FbxVector4& src
);
};
