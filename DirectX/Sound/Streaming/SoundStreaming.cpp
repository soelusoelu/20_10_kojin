#include "SoundStreaming.h"
#include "../Data/SoundData.h"
#include "../Player/SoundPlayer.h"
#include "../Player/SoundPlayTimer.h"
#include "../Player/Buffer/BufferSubmitter.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"
#include "../../System/GlobalFunction.h"
#include <utility>

SoundStreaming::SoundStreaming(SourceVoice& sourceVoice, SoundPlayer& player, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format) :
    mSourceVoice(sourceVoice),
    mPlayer(player),
    mBufferSubmitter(std::make_unique<BufferSubmitter>(sourceVoice)),
    mLoader(std::move(loader)),
    mBuffer{ nullptr, nullptr },
    mPrimary(0),
    mSecondary(1),
    READ_SIZE(format.avgBytesPerSec),
    mRemainBufferSize(0),
    mWrite(0),
    mEndOfFile(false) {
    for (size_t i = 0; i < BUFFER_COUNT; i++) {
        mBuffer[i] = new BYTE[READ_SIZE];
    }
    mRemainBufferSize = mLoader->size() % READ_SIZE;
}

SoundStreaming::~SoundStreaming() {
    for (size_t i = 0; i < BUFFER_COUNT; i++) {
        safeDeleteArray(mBuffer[i]);
    }
}

void SoundStreaming::polling() {
    //ファイルの終端ならこれ以上読み込めない
    if (mEndOfFile) {
        return;
    }

    XAUDIO2_VOICE_STATE state = { 0 };
    mSourceVoice.getXAudio2SourceVoice()->GetState(&state);
    //再生キューがBUFFER_QUEUE_MAX未満なら新たにバッファを追加する
    if (state.BuffersQueued < BUFFER_QUEUE_MAX) {
        std::swap(mPrimary, mSecondary);
        addBuffer();
    }
}

void SoundStreaming::seek(float point) {
    initialize();

    //シーク位置が曲の最初を指しているならここで終わる
    if (Math::nearZero(point)) {
        return;
    }

    const auto& data = mSourceVoice.getSoundData();
    auto sec = data.clamp(point);
    auto seekPoint = static_cast<unsigned>(data.averageBytesPerSec * sec);

    recomputeRemainBufferSize(seekPoint);

    mLoader->seek(static_cast<long>(seekPoint));
    mWrite = seekPoint;
}

void SoundStreaming::addBuffer() {
    long res = 0;
    //次の読み込みがデータサイズを超えるなら
    if (mWrite + READ_SIZE > mLoader->size()) {
        res = read(mRemainBufferSize);
    } else {
        //再生時間を揃えるために無理やり
        if (mWrite == 0) {
            mPlayer.getPlayTimer().setPlayTime(0.f);
        }

        res = read(READ_SIZE);
    }
    mWrite += res;

    //書き込み量がデータサイズを超えたら終了
    if (mWrite >= mLoader->size()) {
        mEndOfFile = true;
    }

    //バッファ作成
    SimpleSoundBuffer buf;
    buf.buffer = mBuffer[mSecondary];
    buf.size = res;
    buf.isEndOfStream = mEndOfFile;

    mBufferSubmitter->submitSimpleBuffer(buf);
}

long SoundStreaming::read(unsigned readSize) {
    return mLoader->read(&mBuffer[mSecondary], readSize);
}

void SoundStreaming::initialize() {
    mLoader->seekBegin();
    mWrite = 0;
    mEndOfFile = false;
    recomputeRemainBufferSize(0);
}

void SoundStreaming::recomputeRemainBufferSize(unsigned point) {
    auto sub = mLoader->size() - point;
    mRemainBufferSize = sub % READ_SIZE;
}
