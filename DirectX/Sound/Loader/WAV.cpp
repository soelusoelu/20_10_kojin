#include "WAV.h"
#include "../../DebugLayer/Debug.h"

WAV::WAV() :
    mHMmio(nullptr),
    mRiffChunk(),
    mFormatChunk(),
    mDataChunk() {
}

WAV::~WAV() {
    mmioClose(mHMmio, 0);
}

bool WAV::loadFromFile(WAVEFORMATEX* format, const std::string& fileName) {
    //WAVファイル内のヘッダー情報(音データ以外)の確認と読み込み
    open(fileName);
    if (!mHMmio) {
        Debug::logWarning("The file cannot be opened.");
        return false;
    }

    //ファイルポインタをRIFFチャンクの先頭にセットする
    if (!descend(&mRiffChunk, nullptr, FindFlag::NONE)) {
        return false;
    }

    //チャンクからWavファイルか確認
    if (!isWavFile(mRiffChunk)) {
        return false;
    }

    //フォーマット作成
    if (!createFormatByChunk(format)) {
        return false;
    }

    //WAVファイル内の音データの読み込み
    setChunkID(&mDataChunk, CHUNK_DATA);
    if (!descend(&mDataChunk, &mRiffChunk, FindFlag::CHUNK)) {
        return false;
    }

    return true;
}

long WAV::read(BYTE** buffer, unsigned size) {
    return readChunk(*buffer, size);
}

void WAV::seek(long offset, Seek seek) {
    mmioSeek(mHMmio, offset, static_cast<int>(seek));
}

void WAV::seekBegin() {
    seek(mDataChunk.dwDataOffset, Seek::BEGIN);
}

unsigned WAV::size() const {
    return mDataChunk.cksize;
}

void WAV::open(const std::string& fileName) {
    char* fn = const_cast<char*>(fileName.c_str()); //他にやり方思い浮かばない
    mHMmio = mmioOpenA(fn, nullptr, MMIO_ALLOCBUF | MMIO_READ);
}

long WAV::readChunk(void* out, unsigned size) const {
    return mmioRead(mHMmio, reinterpret_cast<HPSTR>(out), static_cast<LONG>(size));
}

bool WAV::descend(MMCKINFO* out, const MMCKINFO* parent, FindFlag flag) const {
    auto res = mmioDescend(mHMmio, out, parent, static_cast<unsigned>(flag));
    if (res != MMSYSERR_NOERROR) {
        Debug::logError("Failed descend.");
        return false;
    }
    return true;
}

bool WAV::ascend(MMCKINFO* out) {
    auto res = mmioAscend(mHMmio, out, 0);
    if (res != MMSYSERR_NOERROR) {
        Debug::logError("Failed ascend.");
        return false;
    }
    return true;
}

void WAV::setChunkID(MMCKINFO* out, const char* ch) {
    out->ckid = getFourCC(ch);
}

constexpr FOURCC WAV::getFourCC(const char* ch) const {
    return mmioFOURCC(ch[0], ch[1], ch[2], ch[3]);
}

constexpr bool WAV::isWavFile(const MMCKINFO& riffChunk) const {
    if (riffChunk.ckid != getFourCC(CHUNK_RIFF)) {
        Debug::logWarning("Chunk ID does not match 'riff'.");
        return false;
    }
    if (riffChunk.fccType != getFourCC(CHUNK_WAVE)) {
        Debug::logWarning("FourCC Type does not match 'wave'.");
        return false;
    }
    return true;
}

bool WAV::createFormatByChunk(WAVEFORMATEX* format) {
    //ファイルポインタをfmtチャンクにセットする
    setChunkID(&mFormatChunk, CHUNK_FORMAT);
    if (!descend(&mFormatChunk, &mRiffChunk, FindFlag::CHUNK)) {
        return false;
    }

    //PCMフォーマットを読み込む
    PCMWAVEFORMAT pcmWaveFmt;
    if (readChunk(&pcmWaveFmt, mFormatChunk.cksize) != sizeof(pcmWaveFmt)) {
        Debug::logWarning("Failed to read wav format.");
        return false;
    }

    //フォーマット作成
    createWaveFormat(&format, pcmWaveFmt);
    //if (!ascend(&mFormatChunk)) {
    //    return false;
    //}

    return true;
}

void WAV::createWaveFormat(WAVEFORMATEX** dst, const PCMWAVEFORMAT& src) {
    auto& o = *dst;
    o->wFormatTag = src.wf.wFormatTag;
    o->nChannels = src.wf.nChannels;
    o->nSamplesPerSec = src.wf.nSamplesPerSec;
    o->nAvgBytesPerSec = src.wf.nAvgBytesPerSec;
    o->nBlockAlign = src.wf.nBlockAlign;
    o->wBitsPerSample = src.wBitsPerSample;
    o->cbSize = 0;
}
