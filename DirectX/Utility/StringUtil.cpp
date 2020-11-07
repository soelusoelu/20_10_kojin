#include "StringUtil.h"
#include "../DebugLayer/Debug.h"
#include "../System/SystemInclude.h"
#include <locale.h>
#include <iomanip>
#include <sstream>

std::vector<std::string> StringUtil::split(const std::string& src, const char delimiter) {
    StringArray temp;
    auto s = src;
    auto pos = s.find(delimiter);
    while (pos != std::string::npos) {
        auto sub = s.substr(0, pos);
        temp.emplace_back(sub);
        s = s.substr(pos + 1);
        pos = s.find(delimiter);
    }
    temp.emplace_back(s);

    return temp;
}

const wchar_t* StringUtil::charToWchar(const char* src) {
    wchar_t dst[256];
    //size_t len = 0;
    //setlocale(LC_ALL, "japanese");
    //mbstowcs_s(&len, dst, 256, src, _TRUNCATE);

    MultiByteToWideChar(CP_ACP, 0, src, -1, dst, _countof(dst));

    return dst;
}

std::string StringUtil::wcharToString(const wchar_t* src) {
    char dst[256];
    WideCharToMultiByte(CP_ACP, 0, src, -1, dst, _countof(dst), nullptr, nullptr);

    return dst;
}

std::string StringUtil::boolToString(bool value) {
    return (value) ? "true" : "false";
}

std::string StringUtil::intToString(int number) {
    return std::to_string(number);
}

std::string StringUtil::floatToString(float number, int decimalDigits) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimalDigits) << number;
    return oss.str();
}

std::string StringUtil::vector2ToString(const Vector2& vec, int decimalDigits) {
    auto x = "X " + floatToString(vec.x, decimalDigits);
    auto y = "Y " + floatToString(vec.y, decimalDigits);
    return x + "  " + y;
}

std::string StringUtil::vector3ToString(const Vector3& vec, int decimalDigits) {
    auto x = "X " + floatToString(vec.x, decimalDigits);
    auto y = "Y " + floatToString(vec.y, decimalDigits);
    auto z = "Z " + floatToString(vec.z, decimalDigits);
    return x + "  " + y + "  " + z;
}

std::string StringUtil::vector4ToString(const Vector4& vec, int decimalDigits) {
    auto x = "X " + floatToString(vec.x, decimalDigits);
    auto y = "Y " + floatToString(vec.y, decimalDigits);
    auto z = "Z " + floatToString(vec.z, decimalDigits);
    auto w = "W " + floatToString(vec.w, decimalDigits);
    return x + "  " + y + "  " + z + "  " + w;
}

std::string StringUtil::quaternionToString(const Quaternion& quaternion, int decimalDigits) {
    auto x = "X " + floatToString(quaternion.x, decimalDigits);
    auto y = "Y " + floatToString(quaternion.y, decimalDigits);
    auto z = "Z " + floatToString(quaternion.z, decimalDigits);
    auto w = "W " + floatToString(quaternion.w, decimalDigits);
    return x + "  " + y + "  " + z + "  " + w;
}

std::string StringUtil::anyToString(const std::any& src) {
    if (!src.has_value()) {
        return "";
    }

    const auto& type = src.type();
    std::string str;
    if (type == typeid(int)) {
        auto value = std::any_cast<int>(src);
        str = intToString(value);
    } else if (type == typeid(float)) {
        auto value = std::any_cast<float>(src);
        str = floatToString(value);
    } else if (type == typeid(bool)) {
        auto value = std::any_cast<bool>(src);
        str = boolToString(value);
    } else if (type == typeid(const char*)) {
        str = std::any_cast<const char*>(src);
    } else if (type == typeid(std::string)) {
        str = std::any_cast<std::string>(src);
    } else if (type == typeid(Vector2)) {
        auto value = std::any_cast<Vector2>(src);
        str = vector2ToString(value);
    } else if (type == typeid(Vector3)) {
        auto value = std::any_cast<Vector3>(src);
        str = vector3ToString(value);
    } else if (type == typeid(Vector4)) {
        auto value = std::any_cast<Vector4>(src);
        str = vector4ToString(value);
    } else if (type == typeid(Quaternion)) {
        auto value = std::any_cast<Quaternion>(src);
        str = quaternionToString(value);
    } else {
        Debug::logWarning(src.type().name());
    }

    return str;
}
