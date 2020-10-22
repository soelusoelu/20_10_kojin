#pragma once

#include "TextBase.h"

class TextFloat : public TextBase {
public:
    TextFloat(GameObject& gameObject);
    ~TextFloat();
    virtual void lateUpdate() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void setNumber(float number);
    float number() const;
    void setDecimalDigits(int decimalDigits);
    int getDecimalDigits() const;

private:
    float mNumber;
    int mDecimalDigits;
};
