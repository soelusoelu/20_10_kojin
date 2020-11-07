#pragma once

#include "TextBase.h"
#include <string>

class Text : public TextBase {
public:
    Text(GameObject& gameObject);
    ~Text();
    virtual void lateUpdate() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawInspector() override;
    void setText(const std::string& text);
    const std::string& text() const;

private:
    std::string mText;
};
