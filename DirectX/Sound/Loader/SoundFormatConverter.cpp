#include "SoundFormatConverter.h"
#include "../../DebugLayer/Debug.h"
#include "../../System/GlobalFunction.h"

SoundFormatConverter::SoundFormatConverter() :
    mAcmStream(nullptr),
    mAcmStreamHeader{ 0 } {
}

SoundFormatConverter::~SoundFormatConverter() {
    acmStreamUnprepareHeader(mAcmStream, &mAcmStreamHeader, 0);
    acmStreamClose(mAcmStream, 0);
    safeDeleteArray(mAcmStreamHeader.pbSrc);
    safeDeleteArray(mAcmStreamHeader.pbDst);
}

bool SoundFormatConverter::mp3ToPCM(WAVEFORMATEX** pcmFormat, MPEGLAYER3WAVEFORMAT& mp3Format) {
    //フォーマット変換できるか
    if (!isConvert(pcmFormat, mp3Format)) {
        Debug::logError("Failed mp3 to pcm convert.");
        return false;
    }
    //ACMストリームを開く
    if (!openAcmStream(pcmFormat, mp3Format)) {
        Debug::logError("Failed acm stream opened.");
        return false;
    }
    //ACMストリームヘッダーを作成
    if (!createAcmStreamHeader(pcmFormat, mp3Format)) {
        Debug::logError("Failed created acm stream header.");
        return false;
    }

    return true;
}

unsigned SoundFormatConverter::convert(BYTE** out, unsigned readSize) {
    ZeroMemory(mAcmStreamHeader.pbSrc, mAcmStreamHeader.cbSrcLength);
    memcpy(mAcmStreamHeader.pbSrc, *out, readSize);
    acmStreamConvert(mAcmStream, &mAcmStreamHeader, 0);
    memcpy(*out, mAcmStreamHeader.pbDst, mAcmStreamHeader.cbDstLengthUsed);
    return mAcmStreamHeader.cbDstLengthUsed;
}

bool SoundFormatConverter::decodeSize(const unsigned src, unsigned* dst, bool srcToDst) {
    auto res = acmStreamSize(mAcmStream, src, reinterpret_cast<DWORD*>(dst), (srcToDst) ? ACM_STREAMSIZEF_SOURCE : ACM_STREAMSIZEF_DESTINATION);
    return (res == MMSYSERR_NOERROR);
}

bool SoundFormatConverter::isConvert(WAVEFORMATEX** pcmFormat, MPEGLAYER3WAVEFORMAT& mp3Format) const {
    auto res = acmFormatSuggest(nullptr, &mp3Format.wfx, *pcmFormat, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);
    return (res == MMSYSERR_NOERROR);
}

bool SoundFormatConverter::openAcmStream(WAVEFORMATEX** pcmFormat, MPEGLAYER3WAVEFORMAT& mp3Format) {
    auto res = acmStreamOpen(&mAcmStream, nullptr, &mp3Format.wfx, *pcmFormat, nullptr, 0, 0, ACM_STREAMOPENF_NONREALTIME);
    return (res == MMSYSERR_NOERROR);
}

bool SoundFormatConverter::createAcmStreamHeader(WAVEFORMATEX** pcmFormat, const MPEGLAYER3WAVEFORMAT& mp3Format) {
    const unsigned size = (*pcmFormat)->nAvgBytesPerSec;
    unsigned out = 0;
    decodeSize(size, &out, false);

    mAcmStreamHeader.cbStruct = sizeof(ACMSTREAMHEADER);
    mAcmStreamHeader.pbSrc = new BYTE[out];
    mAcmStreamHeader.cbSrcLength = out;
    mAcmStreamHeader.pbDst = new BYTE[size];
    mAcmStreamHeader.cbDstLength = size;

    auto res = acmStreamPrepareHeader(mAcmStream, &mAcmStreamHeader, 0);
    return (res == MMSYSERR_NOERROR);
}
