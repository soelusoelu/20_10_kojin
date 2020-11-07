#pragma once

#include "../Component.h"

//キャラクター生成コスト管理クラス
class CharacterCost : public Component {
public:
    CharacterCost(GameObject& gameObject);
    ~CharacterCost();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawInspector() override;
    //指定の量コストを使う
    void useCost(int amount);
    //指定の量のコストが戻ってくる
    void returnCost(int amount);
    //残りのコストを取得する
    int getCost() const;
    //最大コストを取得する
    int getMaxCost() const;

private:
    CharacterCost(const CharacterCost&) = delete;
    CharacterCost& operator=(const CharacterCost&) = delete;

    //コストを適正範囲にクランプする
    void clampCost();

private:
    int mMaxCost;
    int mCurrentCost;
};
