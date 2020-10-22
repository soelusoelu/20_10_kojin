#pragma once

#include "../Component.h"
#include <memory>

class SpriteComponent;

class Fade : public Component {
    enum class State {
        FADE_OUT,
        FADE_IN,
        STOP
    };

public:
    Fade(GameObject& gameObject);
    ~Fade();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    //フェードアウト指定
    void fadeOut();
    //フェードイン指定
    void fadeIn();
    //フェードを止める
    void stop();
    //フェード状態を初期化
    void reset();
    //フェードする秒数の設定
    void setFadeSpeed(float sec);
    //フェードアウトが終了したか
    bool isFadeOutEnded() const;
    //フェードインが終了したか
    bool isFadeInEnded() const;

private:
    void fadeOutUpdate();
    void fadeInUpdate();

private:
    std::shared_ptr<SpriteComponent> mSprite;
    State mState;
    float mFadeSpeed;
    bool mIsFadeOutEnded;
    bool mIsFadeInEnded;
};
