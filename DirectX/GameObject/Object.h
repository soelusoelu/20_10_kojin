#pragma once

#include <memory>

class Time;

//すべての基本となるクラス
class Object {
public:
    Object();
    virtual ~Object();
    //削除
    void destroy();
    //sec秒後、削除
    void destroy(float sec);
    //破壊するか
    bool isDead() const;
    //DestroyTimerの更新
    void updateDestroyTimer();

private:
    std::unique_ptr<Time> mDestroyTimer;
    bool mIsDead;
};
