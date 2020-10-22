#pragma once

#include <functional>

class Vector2;

class IButton {
public:
    virtual ~IButton() = default;
    //ボタンの機能を設定
    virtual void setClickFunc(std::function<void()> onClick) = 0;
    //ボタンの位置を返す
    virtual const Vector2& getPosition() const = 0;
    //ハイライト設定
    virtual void setHighlighted(bool set) = 0;
    //現在のハイライト設定を返す
    virtual bool getHighlighted() const = 0;
    //ptがボタンの範囲内ならtrueを返す
    virtual bool containsPoint(const Vector2& pt) const = 0;
    //ボタンが押されたときに呼び出される
    virtual void onClick() = 0;
};
