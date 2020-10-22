#include "SoundCreater.h"
#include "../Loader/MP3.h"
#include "../Loader/WAV.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../Voice/SubmixVoice/SubmixVoice.h"
#include "../XAudio2/SoundBase.h"
#include "../XAudio2/XAudio2.h"
#include "../../DebugLayer/Debug.h"
#include "../../System/World.h"
#include "../../Utility/Directory.h"
#include "../../Utility/FileUtil.h"

SoundCreater::SoundCreater(const SoundBase& base) :
    mSoundBase(base) {
}

SoundCreater::~SoundCreater() = default;

std::shared_ptr<SourceVoice> SoundCreater::createSourceVoice(const std::string& filePath, const SourceVoiceInitParam& param) const {
    //サウンドAPIが使用できない状態ならnullptrを返す
    if (mSoundBase.isNull()) {
        return nullptr;
    }

    //ファイル拡張子からローダーを生成
    auto loader = createLoaderFromFilePath(filePath);
    if (!loader) {
        return nullptr;
    }

    World::instance().directory().setSoundDirectory(filePath);
    auto fileName = FileUtil::getFileNameFromDirectry(filePath);
    //音を読み込む
    WAVEFORMATEX format = { 0 };
    auto res = loader->loadFromFile(&format, fileName);
    if (!res) {
        return nullptr;
    }

    //ソースボイス生成
    return mSoundBase.getXAudio2().createSourceVoice(mSoundBase.getMasteringVoice(), loader, format, param);
}

std::shared_ptr<SubmixVoice> SoundCreater::createSubmixVoice(const SubmixVoiceInitParam& param) const {
    //サウンドAPIが使用できない状態ならnullptrを返す
    if (mSoundBase.isNull()) {
        return nullptr;
    }

    return mSoundBase.getXAudio2().createSubmixVoice(mSoundBase.getMasteringVoice(), param);
}

std::unique_ptr<ISoundLoader> SoundCreater::createLoaderFromFilePath(const std::string& filePath) const {
    std::unique_ptr<ISoundLoader> loader = nullptr;
    auto ext = FileUtil::getFileExtension(filePath);
    if (ext == ".wav") {
        loader = std::make_unique<WAV>();
    } else if (ext == ".mp3") {
        loader = std::make_unique<MP3>();
    } else {
        Debug::windowMessage(filePath + ": 対応していない拡張子です");
    }

    return loader;
}
