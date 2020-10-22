#pragma once

#include "../Component.h"
#include <complex>
#include <memory>
#include <vector>

class SoundComponent;

class WaveformRenderSample : public Component {
public:
    WaveformRenderSample(GameObject& gameObject);
    ~WaveformRenderSample();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<SoundComponent> mSound;
    std::vector<std::complex<float>> mFourierData;
    int mFourierID;
};
