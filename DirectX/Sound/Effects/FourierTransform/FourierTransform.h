#pragma once

#include <complex>
#include <vector>

//フーリエ変換
class FourierTransform {
public:
    FourierTransform();
    ~FourierTransform();

    void initialize(unsigned size);

    /// <summary>
    /// 離散フーリエ変換
    /// </summary>
    /// <param name="out">出力先</param>
    /// <param name="in">入力</param>
    /// <param name="size">長さ</param>
    void discreteFourierTransform(std::complex<float>* out, const std::complex<float>* in, unsigned size) const;

    /// <summary>
    /// 高速フーリエ変換
    /// </summary>
    /// <param name="out">出力先</param>
    /// <param name="in">入力</param>
    /// <param name="size">長さ(2のべき乗のみ)</param>
    void fastFourierTransform(std::complex<float>* out, const std::complex<float>* in, unsigned size) const;

private:
    int log2(int x) const;
    int pow2(int x) const;
    //インデックス並び替えのためのテーブルの作成
    void initBitRevTable(unsigned size);
    //インデックスの並び替え
    void bitReverse(std::complex<float>* fftData, unsigned size) const;

private:
    std::vector<int> mBitRevTable;
    int mNumberOfStage;
};
