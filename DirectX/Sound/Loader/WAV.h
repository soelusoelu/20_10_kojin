#pragma once

#include "ISoundLoader.h"
#include "../../System/SystemInclude.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

class WAV : public ISoundLoader {
    enum class FindFlag {
        NONE = 0,
        CHUNK = MMIO_FINDCHUNK, //指定された識別子を持つチャンクを検索
        RIFF = MMIO_FINDRIFF, //識別子「RIFF」と指定されたチャンクを検索
        LIST = MMIO_FINDLIST, //識別子「LIST」と指定されたチャンクを検索
    };

public:
    WAV();
    ~WAV();
    virtual bool loadFromFile(WAVEFORMATEX* format, const std::string& fileName) override;
    //指定したサイズ分データを読み取る
    virtual long read(BYTE** buffer, unsigned size) override;
    //オフセット分シーク
    virtual void seek(long offset, Seek seek = Seek::CURRENT) override;
    //バッファ部分の先頭までシーク
    virtual void seekBegin() override;
    //波形部分の大きさ
    virtual unsigned size() const override;

private:
    WAV(const WAV&) = delete;
    WAV& operator=(const WAV&) = delete;

    //WAVファイルを開く
    void open(const std::string& fileName);
    //チャンク全体を読み込む
    long readChunk(void* out, unsigned size) const;
    //チャンクを読み込む
    bool descend(MMCKINFO* out, const MMCKINFO* parent, FindFlag flag) const;
    bool ascend(MMCKINFO* out);
    //チャンクを設定する
    void setChunkID(MMCKINFO* out, const char* ch);
    //FourCCを取得する
    constexpr FOURCC getFourCC(const char* ch) const;
    //WAVファイルか
    constexpr bool isWavFile(const MMCKINFO& riffChunk) const;
    //チャンクからフォーマットを作成する
    bool createFormatByChunk(WAVEFORMATEX* format);
    //Waveフォーマットを作成する
    void createWaveFormat(WAVEFORMATEX** dst, const PCMWAVEFORMAT& src);

private:
    //WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPI)
    HMMIO mHMmio;
    //最上部チャンク(RIFFチャンク)保存用
    MMCKINFO mRiffChunk;
    //フォーマットチャンク情報
    MMCKINFO mFormatChunk;
    //データチャンク情報
    MMCKINFO mDataChunk;

    //チャンクIDのサイズ
    static constexpr unsigned char CHUNK_ID_SIZE = 4;
    //チャンクの種類
    static constexpr const char CHUNK_FORMAT[CHUNK_ID_SIZE] = { 'f', 'm', 't', ' ' };
    static constexpr const char CHUNK_DATA[CHUNK_ID_SIZE] = { 'd', 'a', 't', 'a' };
    static constexpr const char CHUNK_RIFF[CHUNK_ID_SIZE] = { 'R', 'I', 'F', 'F' };
    static constexpr const char CHUNK_WAVE[CHUNK_ID_SIZE] = { 'W', 'A', 'V', 'E' };
};
