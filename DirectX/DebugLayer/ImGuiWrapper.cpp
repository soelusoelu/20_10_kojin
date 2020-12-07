#include "ImGuiWrapper.h"

bool ImGuiWrapper::dragInt(const std::string& label, int& v, float speed, int min, int max, const char* format, ImGuiSliderFlags flags) {
    return ImGui::DragInt(label.c_str(), &v, speed, min, max, format, flags);
}

bool ImGuiWrapper::dragFloat(const std::string& label, float& v, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) {
    return ImGui::DragFloat(label.c_str(), &v, speed, min, max, format, flags);
}

bool ImGuiWrapper::dragVector2(const std::string& label, Vector2& v, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) {
    float temp[2] = { v.x, v.y };
    if (ImGui::DragFloat2(label.c_str(), temp, speed, min, max, format, flags)) {
        static_assert(sizeof(v) == sizeof(temp));
        memcpy_s(&v, sizeof(v), temp, sizeof(temp));
        return true;
    }

    return false;
}

bool ImGuiWrapper::dragVector3(const std::string& label, Vector3& v, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) {
    float temp[3] = { v.x, v.y, v.z };
    if (ImGui::DragFloat3(label.c_str(), temp, speed, min, max, format, flags)) {
        static_assert(sizeof(v) == sizeof(temp));
        memcpy_s(&v, sizeof(v), temp, sizeof(temp));
        return true;
    }

    return false;
}

bool ImGuiWrapper::dragVector4(const std::string& label, Vector4& v, float speed, float min, float max, const char* format, ImGuiSliderFlags flags) {
    float temp[4] = { v.x, v.y, v.z, v.w };
    if (ImGui::DragFloat4(label.c_str(), temp, speed, min, max, format, flags)) {
        static_assert(sizeof(v) == sizeof(temp));
        memcpy_s(&v, sizeof(v), temp, sizeof(temp));
        return true;
    }

    return false;
}

bool ImGuiWrapper::sliderInt(const std::string& label, int& v, int min, int max, const char* format, ImGuiSliderFlags flags) {
    constexpr int SLIDER_INT_MAX = INT_MAX / 2;
    if (max > SLIDER_INT_MAX) {
        max = SLIDER_INT_MAX;
    }
    return ImGui::SliderInt(label.c_str(), &v, min, max, format, flags);
}

bool ImGuiWrapper::sliderFloat(const std::string& label, float& v, float min, float max, const char* format, ImGuiSliderFlags flags) {
    return ImGui::SliderFloat(label.c_str(), &v, min, max, format, flags);
}

bool ImGuiWrapper::sliderVector2(const std::string& label, Vector2& v, float min, float max, const char* format, ImGuiSliderFlags flags) {
    float temp[2] = { v.x, v.y };
    if (ImGui::SliderFloat2(label.c_str(), temp, min, max, format, flags)) {
        static_assert(sizeof(v) == sizeof(temp));
        memcpy_s(&v, sizeof(v), temp, sizeof(temp));
        return true;
    }

    return false;
}

bool ImGuiWrapper::sliderVector3(const std::string& label, Vector3& v, float min, float max, const char* format, ImGuiSliderFlags flags) {
    float temp[3] = { v.x, v.y, v.z };
    if (ImGui::SliderFloat3(label.c_str(), temp, min, max, format, flags)) {
        static_assert(sizeof(v) == sizeof(temp));
        memcpy_s(&v, sizeof(v), temp, sizeof(temp));
        return true;
    }

    return false;
}

bool ImGuiWrapper::sliderVector4(const std::string& label, Vector4& v, float min, float max, const char* format, ImGuiSliderFlags flags) {
    float temp[4] = { v.x, v.y, v.z, v.w };
    if (ImGui::SliderFloat4(label.c_str(), temp, min, max, format, flags)) {
        static_assert(sizeof(v) == sizeof(temp));
        memcpy_s(&v, sizeof(v), temp, sizeof(temp));
        return true;
    }

    return false;
}

bool ImGuiWrapper::colorEdit3(const std::string& label, Vector3& col, ImGuiColorEditFlags flags) {
    float temp[3] = { col.x, col.y, col.z };
    if (ImGui::ColorEdit3(label.c_str(), temp, flags)) {
        static_assert(sizeof(col) == sizeof(temp));
        memcpy_s(&col, sizeof(col), temp, sizeof(temp));
        return true;
    }

    return false;
}

bool ImGuiWrapper::colorEdit4(const std::string& label, Vector4& col, ImGuiColorEditFlags flags) {
    float temp[4] = { col.x, col.y, col.z, col.w };
    if (ImGui::ColorEdit4(label.c_str(), temp, flags)) {
        static_assert(sizeof(col) == sizeof(temp));
        memcpy_s(&col, sizeof(col), temp, sizeof(temp));
        return true;
    }

    return false;
}

bool ImGuiWrapper::colorPicker3(const std::string& label, Vector3& col, ImGuiColorEditFlags flags) {
    float temp[3] = { col.x, col.y, col.z };
    if (ImGui::ColorPicker3(label.c_str(), temp, flags)) {
        static_assert(sizeof(col) == sizeof(temp));
        memcpy_s(&col, sizeof(col), temp, sizeof(temp));
        return true;
    }

    return false;
}

bool ImGuiWrapper::colorPicker4(const std::string& label, Vector4& col, ImGuiColorEditFlags flags, const float* ref_col) {
    float temp[4] = { col.x, col.y, col.z, col.w };
    if (ImGui::ColorPicker4(label.c_str(), temp, flags)) {
        static_assert(sizeof(col) == sizeof(temp));
        memcpy_s(&col, sizeof(col), temp, sizeof(temp));
        return true;
    }

    return false;
}

bool ImGuiWrapper::colorButton(const std::string& desc_id, const Vector4& col, ImGuiColorEditFlags flags, const Vector2& size) {
    ImVec4 temp(col.x, col.y, col.z, col.w);
    ImVec2 s(size.x, size.y);
    return ImGui::ColorButton(desc_id.c_str(), temp, flags, s);
}
