#pragma once

#include "TextBase.h"

class TextNumber : public TextBase {
public:
    TextNumber(GameObject& gameObject);
    ~TextNumber();
    virtual void lateUpdate() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawInspector() override;
    void setNumber(int number);
    int number() const;

private:
    int mNumber;
};
