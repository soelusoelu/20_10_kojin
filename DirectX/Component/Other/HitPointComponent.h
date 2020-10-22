#pragma once

#include "../Component.h"

class HitPointComponent : public Component {
public:
    HitPointComponent(GameObject& gameObject);
    ~HitPointComponent();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    //ダメージに応じてHPを減らす
    void takeDamage(int damage);
    //ヒール量に応じてHPを増やす
    void takeHeal(int heal);
    //HPを設定する
    void setHP(int hp, bool isChangeMax = true);
    //現在HPの取得
    int hp() const;
    //最大HPに対しての現在HPの比率(0 ～ 1)
    float hpRate() const;

private:
    //HPが最大HPを越したら調整
    void clampHpIfOverMax();

private:
    int mHP;
    int mMaxHP;
};

