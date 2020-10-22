#pragma once

#include <list>
#include <memory>

class SourceVoice;
class SubmixVoice;
class MasteringVoice;
class Sound3DCalculator;
class Sound3DListener;

class SoundManager {
    using SoundPtr = std::shared_ptr<SourceVoice>;
    using SoundPtrList = std::list<SoundPtr>;
    using SubmixVoicePtr = std::shared_ptr<SubmixVoice>;
    using SubmixVoicePtrList = std::list<SubmixVoicePtr>;

public:
    SoundManager(const MasteringVoice& masteringVoice);
    ~SoundManager();
    void update();
    void add(const SoundPtr& sound);
    void add(const SubmixVoicePtr& submixVoice);
    void setListener(const std::shared_ptr<Sound3DListener>& listener);

private:
    SoundPtrList mSounds;
    SubmixVoicePtrList mSubmixVoices;
    std::unique_ptr<Sound3DCalculator> mCalculator;
    std::shared_ptr<Sound3DListener> mListener;
};
