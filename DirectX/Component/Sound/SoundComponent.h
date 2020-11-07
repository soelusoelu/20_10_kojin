#pragma once

#include "../Component.h"
#include "../../Sound/Data/SoundData.h"
#include "../../Sound/Voice/IVoice.h"
#include "../../Sound/Voice/VoiceDetails.h"
#include <memory>
#include <string>
#include <vector>

class SourceVoice;
class SoundPlayer;
class SoundVolume;
class SoundEffect;
class OutputVoices;

class SoundComponent : public Component {
public:
    SoundComponent(GameObject& gameObject);
    ~SoundComponent();
    virtual void awake() override;
    virtual void update() override;
    virtual void finalize() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawInspector() override;

    //使用可能状態か
    bool isNull() const;

    const VoiceDetails& getVoiceDetails() const;
    SoundData& getSoundData() const;
    SoundPlayer& getSoundPlayer() const;
    SoundVolume& getSoundVolume() const;
    OutputVoices& getOutputVoices() const;
    SoundEffect& getSoundEffect() const;

private:
    std::shared_ptr<SourceVoice> mSound;
    std::string mFileName;
    bool mUse3DSound;
};
