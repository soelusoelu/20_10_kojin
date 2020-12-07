#pragma once

//メッシュ描画インターフェース
class IMeshDrawer {
public:
    virtual ~IMeshDrawer() = default;
    //メッシュを描画する
    virtual void draw(unsigned meshIndex) const = 0;
};
