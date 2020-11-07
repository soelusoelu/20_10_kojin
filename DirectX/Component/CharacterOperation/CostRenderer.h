#pragma once

#include "../Component.h"
#include <memory>

class CharacterCost;
class Text;

//キャラクターのコストを描画するクラス
class CostRenderer : public Component {
public:
    CostRenderer(GameObject& gameObject);
    ~CostRenderer();
    virtual void start() override;
    virtual void update() override;

private:
    CostRenderer(const CostRenderer&) = delete;
    CostRenderer& operator=(const CostRenderer&) = delete;

private:
    std::shared_ptr<CharacterCost> mCost;
    std::shared_ptr<Text> mText;
};
