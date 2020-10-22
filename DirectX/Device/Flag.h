#pragma once

class Flag {
public:
    Flag();
    ~Flag();
    //フラグを立てる
    void set(unsigned value);
    //フラグを折る
    void reset(unsigned value);
    //フラグが立っているか
    bool check(unsigned value) const;
    //全フラグを返す
    unsigned get() const;
    //全フラグを折る
    void clear();

private:
    unsigned mFlags;
};
