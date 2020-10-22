#include "MP3.h"
#include "FrameHeader.h"
#include "SoundFormatConverter.h"
#include "../../DebugLayer/Debug.h"
#include <cassert>
#include <filesystem>
#include <string>

MP3::MP3() :
    mIfs(),
    mFrameHeader(std::make_unique<FrameHeader>()),
    mFormatConverter(std::make_unique<SoundFormatConverter>()),
    mFileSize(0),
    mDataSize(0),
    mDataOffset(0) {
}

MP3::~MP3() = default;

bool MP3::loadFromFile(WAVEFORMATEX* format, const std::string& fileName) {
    mIfs.open(fileName, std::ios::in | std::ios::binary);
    if (!mIfs.is_open()) {
        Debug::logError("Failed " + fileName + " opened.");
    }

    //ファイル全体のサイズ
    mFileSize = std::filesystem::file_size(fileName);

    //ファイルを解析する
    if (!parse()) {
        return false;
    }

    //解析したデータをMPEGLAYER3WAVEFORMATに詰め込む
    MPEGLAYER3WAVEFORMAT mp3wf = { 0 };
    mFrameHeader->setDataToMpegLayer3WaveFormat(&mp3wf);

    //mp3からpcmへデータ変換する
    format->wFormatTag = WAVE_FORMAT_PCM;
    format->cbSize = sizeof(WAVEFORMATEX);
    mFormatConverter->mp3ToPCM(&format, mp3wf);

    //データサイズをデコード
    mFormatConverter->decodeSize(mDataSize, &mDataSize);

    //ファイルポインタをデータの先頭に戻しておく
    seekBegin();

    return true;
}

long MP3::read(BYTE** buffer, unsigned size) {
    //pcm用のサイズからmp3用のサイズに変更する
    unsigned decodeSize = 0;
    mFormatConverter->decodeSize(size, &decodeSize, false);
    //mp3用のサイズからデータを読み取る
    mIfs.read(reinterpret_cast<char*>(*buffer), decodeSize);
    //mp3のバッファからpcmのバッファに変更する
    auto count = mIfs.gcount();
    auto readSize = mFormatConverter->convert(buffer, count);

    return readSize;
}

void MP3::seek(long offset, Seek seek) {
    //pcm用のサイズからmp3用のサイズに変更する
    unsigned decodeOffset = 0;
    mFormatConverter->decodeSize(offset, &decodeOffset, false);

    //フラグをクリアしてシーク
    mIfs.clear();
    mIfs.seekg(decodeOffset, static_cast<std::ios_base::_Seekdir>(seek));
}

void MP3::seekBegin() {
    seek(mDataOffset, Seek::BEGIN);
}

unsigned MP3::size() const {
    return mDataSize;
}

bool MP3::parse() {
    bool initFrameHeader = false;
    bool initData = false;
    BYTE header[ID3V2_HEADER_SIZE];
    while (!mIfs.eof()) {
        mIfs.read(reinterpret_cast<char*>(header), ID3V2_HEADER_SIZE);

        if (mFrameHeader->isFrameHeader(header)) {
            //すでに解析済みなら次へ
            if (initFrameHeader) {
                continue;
            }
            //フレームヘッダの解析をする
            initFrameHeader = mFrameHeader->parseFrameHeader(header);
        } else {
            //すでに解析済みなら次へ
            if (initData) {
                continue;
            }
            //データ部分の解析をする
            initData = parseData(header);
        }

        //必要な解析が完了していたら終了
        if (initFrameHeader && initData) {
            return true;
        }
    }

    //解析失敗
    return false;
}

bool MP3::parseData(const BYTE* header) {
    //ID3v2である
    if (memcmp(header, "ID3", 3) == 0) {
        unsigned tagSize = ((header[6] << 21) | (header[7] << 14) | (header[8] << 7) | (header[9])) + ID3V2_HEADER_SIZE;
        //データ部分へのオフセット値決定
        mDataOffset = tagSize;
        //データ部分のサイズ決定
        mDataSize = mFileSize - mDataOffset;

        return true;
    }
    //ID3v1である
    else {
        //ここに入り次第処理書く
        assert(false);
    }

    return false;
}
