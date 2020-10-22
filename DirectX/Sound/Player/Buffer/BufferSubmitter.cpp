#include "BufferSubmitter.h"
#include "../../Voice/SourceVoice/SourceVoice.h"
#include "../../../DebugLayer/Debug.h"

BufferSubmitter::BufferSubmitter(SourceVoice& sourceVoice) :
    mSourceVoice(sourceVoice) {
}

BufferSubmitter::~BufferSubmitter() = default;

void BufferSubmitter::submitBuffer(const SoundBuffer& buffer) const {
    submitSourceBuffer(toBuffer(buffer));
}

void BufferSubmitter::submitSimpleBuffer(const SimpleSoundBuffer& buffer) const {
    submitSourceBuffer(toBuffer(buffer));
}

void BufferSubmitter::submitSourceBuffer(const XAUDIO2_BUFFER& buffer) const {
    auto res = mSourceVoice.getXAudio2SourceVoice()->SubmitSourceBuffer(&buffer, nullptr);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed submit source buffer.");
    }
#endif // _DEBUG
}

XAUDIO2_BUFFER BufferSubmitter::toBuffer(const SoundBuffer& buffer) const {
    const unsigned sampleRate = mSourceVoice.getVoiceDetails().sampleRate;

    XAUDIO2_BUFFER buf = { 0 };
    buf.Flags = (buffer.isEndOfStream) ? XAUDIO2_END_OF_STREAM : 0;
    buf.AudioBytes = buffer.size;
    buf.pAudioData = buffer.buffer;
    buf.PlayBegin = static_cast<unsigned>(buffer.playBegin * sampleRate);
    buf.PlayLength = static_cast<unsigned>(buffer.playLength * sampleRate);
    buf.LoopBegin = static_cast<unsigned>(buffer.loopBegin * sampleRate);
    buf.LoopLength = static_cast<unsigned>(buffer.loopLength * sampleRate);
    buf.LoopCount = buffer.loopCount;
    buf.pContext = buffer.context;

    return buf;
}

XAUDIO2_BUFFER BufferSubmitter::toBuffer(const SimpleSoundBuffer& buffer) const {
    XAUDIO2_BUFFER buf = {
        (buffer.isEndOfStream) ? XAUDIO2_END_OF_STREAM : 0,
        buffer.size,
        buffer.buffer,
        0, 0, 0, 0, 0,
        nullptr
    };

    return buf;
}
