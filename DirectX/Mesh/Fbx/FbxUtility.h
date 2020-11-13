#pragma once

#include "../../Math/Math.h"
#include <fbxsdk.h>

//Fbxユーティリティ関数群
namespace FbxUtility {
//FbxMatirxからMatrix4へ変換する
Matrix4 substitutionMatrix(
    const FbxMatrix& src
);
};
