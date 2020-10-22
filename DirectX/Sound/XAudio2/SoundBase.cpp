#include "SoundBase.h"
#include "XAudio2.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../System/GlobalFunction.h"
#include <xaudio2.h>
#include <cassert>

SoundBase::SoundBase() :
    mXAudio2(nullptr),
    mMasteringVoice(nullptr),
    mSucceedInitialize(false) {

    //インスタンスを1つに制限
    assert(!mInstantiated);
    mInstantiated = true;

    mSucceedInitialize = comInitialize();
    if (!mSucceedInitialize) {
        return;
    }

    mXAudio2 = new XAudio2(&mSucceedInitialize);
    if (!mSucceedInitialize) {
        CoUninitialize();
        return;
    }

    mMasteringVoice = mXAudio2->createMasteringVoice();
    if (!mMasteringVoice) {
        CoUninitialize();
        mSucceedInitialize = false;
    }
}

SoundBase::~SoundBase() {
    safeDelete(mMasteringVoice);
    safeDelete(mXAudio2);
    if (!mSucceedInitialize) {
        CoUninitialize();
    }

    mInstantiated = false;
}

XAudio2& SoundBase::getXAudio2() const {
    return *mXAudio2;
}

MasteringVoice& SoundBase::getMasteringVoice() const {
    return *mMasteringVoice;
}

bool SoundBase::isNull() const {
    return !mSucceedInitialize;
}

bool SoundBase::comInitialize() {
    if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
        Debug::logError("Failed sound com initialized.");
        return false;
    }
    return true;
}
