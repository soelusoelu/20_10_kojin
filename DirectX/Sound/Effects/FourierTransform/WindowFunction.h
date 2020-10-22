#pragma once

#include <complex>
#include <vector>

//窓関数
class WindowFunction {
public:
    /// <summary>
    /// ハニング窓の波形を返す
    /// </summary>
    /// <param name="out">波形の出力先</param>
    /// <param name="N">波形の粒度</param>
    static void getHanningWave(float* out, unsigned size);

    /// <summary>
    /// ハニング窓の波形をvector型で返す
    /// </summary>
    /// <param name="size">波形の粒度</param>
    /// <returns>ハニング窓の波形</returns>
    static std::vector<float> getHanningWave(unsigned size);

    /// <summary>
    /// 波形にハニング窓を掛ける
    /// </summary>
    /// <param name="data">ハニング窓を掛けたい波形</param>
    /// <param name="size">波形の長さ</param>
    static void hanning(float* data, unsigned size);

    /// <summary>
    /// 波形にハニング窓を掛ける
    /// </summary>
    /// <param name="out">ハニング窓を掛けた波形の出力先</param>
    /// <param name="in">ハニング窓を掛けたい波形</param>
    /// <param name="size">波形の長さ</param>
    static void hanning(std::complex<float>* out, const float* in, unsigned size);

private:
    WindowFunction() = delete;
    ~WindowFunction() = delete;

    //ハニング窓の計算式
    static float hanningFormula(unsigned n, unsigned size);
};
