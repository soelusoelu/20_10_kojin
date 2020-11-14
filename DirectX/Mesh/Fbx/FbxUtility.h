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
};
