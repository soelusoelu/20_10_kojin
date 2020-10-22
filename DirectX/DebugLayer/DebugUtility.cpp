#include "DebugUtility.h"
#include "FixedDebugInformation.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "LineRenderer/LineRenderer2D.h"
#include "LineRenderer/LineRenderer3D.h"
#include "Log.h"
#include "Pause.h"
#include "../Device/DrawString.h"
#include "../System/GlobalFunction.h"
#include "../System/SystemInclude.h"

void DebugUtility::create() {
    mDrawString = new DrawString();
    mLog = new Log();
    mFixedDebugInfo = new FixedDebugInformation(mDrawString);
    mHierarchy = new Hierarchy(mDrawString);
    mInspector = new Inspector(mDrawString);
    mPause = new Pause();
    mLineRenderer2D = new LineRenderer2D();
    mLineRenderer3D = new LineRenderer3D();
}

void DebugUtility::loadProperties(const rapidjson::Value& inObj) {
    mDrawString->loadProperties(inObj);
    mLog->loadProperties(inObj);
    mFixedDebugInfo->loadProperties(inObj);
    mHierarchy->loadProperties(inObj);
    mInspector->loadProperties(inObj);
    mPause->loadProperties(inObj);
}

void DebugUtility::initialize() {
    mDrawString->initialize();
    mLog->initialize();
    mFixedDebugInfo->initialize();
    mHierarchy->initialize();
    mInspector->initialize();
    mPause->initialize();
    mLineRenderer2D->initialize();
    mLineRenderer3D->initialize();
}

void DebugUtility::finalize() {
    safeDelete(mLineRenderer3D);
    safeDelete(mLineRenderer2D);
    safeDelete(mPause);
    safeDelete(mInspector);
    safeDelete(mHierarchy);
    safeDelete(mFixedDebugInfo);
    safeDelete(mLog);
    safeDelete(mDrawString);
}

void DebugUtility::update() {
    mHierarchy->update();
    mPause->update();
}

void DebugUtility::windowMessage(const std::string& message) {
    MessageBoxA(0, message.c_str(), 0, MB_OK);
}

void DebugUtility::draw(const Matrix4& proj) {
    mLog->drawLogs(mDrawString);
    mFixedDebugInfo->draw();
    mHierarchy->drawActors();
    mInspector->drawInspect();
    mPause->drawButton(proj);
    mDrawString->drawAll(proj);
}

void DebugUtility::drawStringClear() {
    mDrawString->clear();
}

Log& DebugUtility::log() {
    return *mLog;
}

FixedDebugInformation* DebugUtility::fixedDebugInfo() {
    return mFixedDebugInfo;
}

Hierarchy& DebugUtility::hierarchy() {
    return *mHierarchy;
}

Inspector& DebugUtility::inspector() {
    return *mInspector;
}

Pause& DebugUtility::pause() {
    return *mPause;
}

LineRenderer2D& DebugUtility::lineRenderer2D() {
    return *mLineRenderer2D;
}

LineRenderer3D& DebugUtility::lineRenderer3D() {
    return *mLineRenderer3D;
}
