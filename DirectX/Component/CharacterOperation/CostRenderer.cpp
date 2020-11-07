#include "CostRenderer.h"
#include "CharacterCost.h"
#include "../Text/Text.h"
#include "../../Utility/StringUtil.h"

CostRenderer::CostRenderer(GameObject& gameObject)
    : Component(gameObject)
    , mCost(nullptr)
    , mText(nullptr)
{
}

CostRenderer::~CostRenderer() = default;

void CostRenderer::start() {
    mCost = getComponent<CharacterCost>();
    mText = getComponent<Text>();
}

void CostRenderer::update() {
    mText->setText(StringUtil::intToString(mCost->getCost()) + "/" + StringUtil::intToString(mCost->getMaxCost()));
}
