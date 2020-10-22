#include "SoundManager.h"
#include "../3D/Sound3DCalculator.h"
#include "../3D/Listener/Sound3DListener.h"
#include "../Player/SoundPlayer.h"
#include "../Voice/SourceVoice/SourceVoice.h"

SoundManager::SoundManager(const MasteringVoice& masteringVoice) :
    mCalculator(std::make_unique<Sound3DCalculator>(masteringVoice)),
    mListener(nullptr) {
}

SoundManager::~SoundManager() {
    mSounds.clear();
    mSubmixVoices.clear();
}

void SoundManager::update() {
    //不要なソースボイスを削除する
    //ちゃんと動くかわからない
    auto itr = mSounds.begin();
    while (itr != mSounds.end()) {
        if (itr->use_count() == 1) {
            itr = mSounds.erase(itr);
        } else {
            ++itr;
        }
    }
    //不要なサブミックスボイスを削除する
    auto itr2 = mSubmixVoices.begin();
    while (itr2 != mSubmixVoices.end()) {
        if (itr2->use_count() == 1) {
            itr2 = mSubmixVoices.erase(itr2);
        } else {
            ++itr2;
        }
    }

    //設定されてるリスナーの更新
    if (mListener) {
        mListener->update();
    }

    //全ソースボイスの更新
    for (const auto& sound : mSounds) {
        sound->update();

        //3D演算
        if (sound->isCalculate3D() && mListener) {
            mCalculator->calculate(mListener->getListener(), sound->getEmitter());
        }
    }
}

void SoundManager::add(const SoundPtr& sound) {
    mSounds.emplace_back(sound);
}

void SoundManager::add(const SubmixVoicePtr& submixVoice) {
    mSubmixVoices.emplace_back(submixVoice);
}

void SoundManager::setListener(const std::shared_ptr<Sound3DListener>& listener) {
    mListener = listener;
}
