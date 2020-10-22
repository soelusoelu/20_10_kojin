#pragma once

#include <xaudio2.h>
#include <string>

//サウンド読み込み用インターフェース
class ISoundLoader {
public:
    enum class Seek {
        BEGIN = SEEK_SET, //ファイルの先頭から
        CURRENT = SEEK_CUR, //現在のファイル位置から
        END = SEEK_END //ファイルの終わりから
    };

public:
    virtual ~ISoundLoader() = default;
    virtual bool loadFromFile(WAVEFORMATEX* format, const std::string& fileName) = 0;
    virtual long read(BYTE** buffer, unsigned size) = 0;
    virtual void seek(long offset, Seek seek = Seek::CURRENT) = 0;
    virtual void seekBegin() = 0;
    virtual unsigned size() const = 0;
};
