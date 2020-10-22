#include "WaveformOutput.h"
#include "../../DebugLayer/Debug.h"
#include "../../System/World.h"
#include "../../Utility/Directory.h"
#include <fstream>

void WaveformOutput::outputWaveform(const std::string& filePath, const float* waveform, unsigned size) {
    //ファイルの出力先を指定する
    World::instance().directory().setRootDirectory();

    std::ofstream ofs(filePath);
    //ファイルパスが無効なら終了
    if (!ofs) {
        Debug::logError(filePath + ": Failed open file.");
        return;
    }

    //ファイルにデータを出力する
    for (size_t i = 0; i < size; i++) {
        ofs << waveform[i] << std::endl;
    }
}

void WaveformOutput::outputComplexes(const std::string& filePath, const std::complex<float>* complexes, unsigned size) {
    //ファイルの出力先を指定する
    World::instance().directory().setRootDirectory();

    std::ofstream ofs(filePath);
    //ファイルパスが無効なら終了
    if (!ofs) {
        Debug::logError(filePath + ": Failed open file.");
        return;
    }

    //ファイルにデータを出力する
    for (size_t i = 0; i < size; i++) {
        ofs << complexes[i].imag() << std::endl;
    }
}
