#include "FPSCounter.h"
#include "Time.h"
#include "../DebugLayer/DebugUtility.h"
#include "../DebugLayer/FixedDebugInformation.h"
#include "../Utility/LevelLoader.h"

FPSCounter::FPSCounter() :
    mDrawUpdateTimer(std::make_unique<Time>(0.5f)),
    mFixedFrame(60.f),
    mCurrentFPS(60.f),
    mFrequency(),
    mPreviousTime() {
}

FPSCounter::~FPSCounter() = default;

void FPSCounter::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getFloat(inObj, "fps", &mFixedFrame);
}

void FPSCounter::fixedFrame() {
    float time = 0.f;
    float fix = 1000.f / mFixedFrame;

    while (time < fix) {
        QueryPerformanceFrequency(&mFrequency);

        time = static_cast<float>(Time::time() - mPreviousTime);
        time *= 1000.f / static_cast<float>(mFrequency.QuadPart);
    }
    mPreviousTime = Time::time();

    float deltaTime = static_cast<float>(time / 1000.f);
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }
    Time::deltaTime = deltaTime;

#ifdef _DEBUG
    drawFPS(time);
#endif // _DEBUG
}

void FPSCounter::setFixedFrame(float fixedFrame) {
    mFixedFrame = fixedFrame;
}

void FPSCounter::drawFPS(float time) {
    mDrawUpdateTimer->update();
    if (mDrawUpdateTimer->isTime()) {
        mDrawUpdateTimer->reset();
        mCurrentFPS = 1000.f / time;
    }
    DebugUtility::fixedDebugInfo()->drawFPS(mCurrentFPS);
}
