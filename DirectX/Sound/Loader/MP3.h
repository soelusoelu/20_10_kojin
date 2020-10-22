#pragma once

#include "ISoundLoader.h"
#include <mmreg.h>
#include <fstream>
#include <memory>

class FrameHeader;
class SoundFormatConverter;

class MP3 : public ISoundLoader {
public:
    MP3();
    ~MP3();
    virtual bool loadFromFile(WAVEFORMATEX* format, const std::string& fileName) override;
    virtual long read(BYTE** buffer, unsigned size) override;
    virtual void seek(long offset, Seek seek = Seek::CURRENT) override;
    virtual void seekBegin() override;
    virtual unsigned size() const override;

private:
    MP3(const MP3&) = delete;
    MP3& operator=(const MP3&) = delete;

    //ファイルをループして解析する
    bool parse();
    //データ部分の解析をする
    bool parseData(const BYTE* header);

private:
    std::ifstream mIfs;
    std::unique_ptr<FrameHeader> mFrameHeader;
    std::unique_ptr<SoundFormatConverter> mFormatConverter;
    //ファイル全体のサイズ
    unsigned mFileSize;
    //データ部分のサイズ
    unsigned mDataSize;
    //ファイルの先頭からデータへのオフセット値
    unsigned mDataOffset;

    //ID3v2ヘッダのサイズ
    static constexpr unsigned ID3V2_HEADER_SIZE = 10;
};
