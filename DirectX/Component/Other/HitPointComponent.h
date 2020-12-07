#pragma once

#include "../Component.h"
#include <functional>
#include <memory>

class Subject;

class HitPointComponent : public Component {
public:
    HitPointComponent(GameObject& gameObject);
    ~HitPointComponent();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawInspector() override;

    //ダメージに応じてHPを減らす
    void takeDamage(int damage);
    //ヒール量に応じてHPを増やす
    void takeHeal(int heal);
    //HPを設定する
    void setHP(int hp, bool isChangeMax = true);
    //現在HPを取得する
    int getHP() const;
    //最大HPを取得する
    int getMaxHP() const;
    //最大HPに対しての現在HPの比率を取得する[0, 1]
    float hpRate() const;
    //HPが更新された際のコールバック
    void callbackUpdateHP(const std::function<void()>& callback);

private:
    HitPointComponent(const HitPointComponent&) = delete;
    HitPointComponent& operator=(const HitPointComponent&) = delete;

    //HPが最大HPを越したら調整
    void clampHpIfOverMax();

private:
    int mHp;
    int mMaxHp;
    std::unique_ptr<Subject> mCallbackUpdateHp;
};
