#pragma once

#include <vector>

//サイン波生成クラス
class SineWaveGenerator {
public:
    /// <summary>
    /// サイン波を生成する
    /// </summary>
    /// <param name="frequency">周波数</param>
    /// <param name="sampleRate">サンプリングレート</param>
    /// <param name="amplitude">振幅(音量)</param>
    /// <param name="length">音の長さ</param>
    /// <returns>サイン波の配列</returns>
    static std::vector<float> generate(float frequency, float sampleRate = 44100.f, float amplitude = 1.f, unsigned length = 64);

private:
    SineWaveGenerator() = delete;
    ~SineWaveGenerator() = delete;
};
