#include "Time.h"
#include "../System/SystemInclude.h"

Time::Time(float sec) :
    mTimeRate(0.f),
    mLimitTime(sec),
    mIsOverLimit(false) {
}

Time::~Time() = default;

void Time::update() {
    mTimeRate += Time::deltaTime;

    if (mTimeRate >= mLimitTime) {
        mIsOverLimit = true;
    }
}

void Time::reset() {
    mTimeRate = 0.f;
    mIsOverLimit = false;
}

bool Time::isTime() const {
    return mIsOverLimit;
}

void Time::setLimitTime(float sec) {
    mLimitTime = sec;
}

void Time::setCurrentTime(float sec) {
    mTimeRate = sec;
}

float Time::countUpTime() const {
    return mTimeRate;
}

float Time::countDownTime() const {
    return mLimitTime - mTimeRate;
}

float Time::limitTime() const {
    return mLimitTime;
}

float Time::rate() const {
    return mTimeRate / mLimitTime;
}

unsigned long long Time::time() {
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return static_cast<unsigned long long>(time.QuadPart);
}
