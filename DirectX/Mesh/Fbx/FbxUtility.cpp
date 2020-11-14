#include "FbxUtility.h"

Matrix4 FbxUtility::substitutionMatrix(const FbxMatrix& src) {
    Matrix4 dst;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            dst.m[i][j] = static_cast<float>(src.mData[i][j]);
        }
    }

    return dst;
}

Vector3 FbxUtility::fbxDouble3ToVector3(const FbxDouble3& src) {
    Vector3 dst;
    dst.x = static_cast<float>(src[0]);
    dst.y = static_cast<float>(src[1]);
    dst.z = static_cast<float>(src[2]);

    return dst;
}
