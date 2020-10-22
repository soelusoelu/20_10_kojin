#include "Debug.h"
#include "DebugUtility.h"
#include "LineRenderer/LineRenderer2D.h"
#include "LineRenderer/LineRenderer3D.h"
#include "Log.h"

void Debug::windowMessage(const std::string& message) {
#ifdef _DEBUG
    DebugUtility::windowMessage(message);
#endif // _DEBUG
}

void Debug::log(const std::string& message) {
#ifdef _DEBUG
    DebugUtility::log().log(message);
#endif // _DEBUG
}

void Debug::logError(const std::string& message) {
#ifdef _DEBUG
    DebugUtility::log().logError(message);
#endif // _DEBUG
}

void Debug::logWarning(const std::string& message) {
#ifdef _DEBUG
    DebugUtility::log().logWarning(message);
#endif // _DEBUG
}

void Debug::renderLine(const Vector2& p1, const Vector2& p2, const Vector3& color) {
#ifdef _DEBUG
    DebugUtility::lineRenderer2D().renderLine(p1, p2, color);
#endif // _DEBUG
}

void Debug::renderLine(const Vector3& p1, const Vector3& p2, const Vector3& color) {
#ifdef _DEBUG
    DebugUtility::lineRenderer3D().renderLine(p1, p2, color);
#endif // _DEBUG
}
