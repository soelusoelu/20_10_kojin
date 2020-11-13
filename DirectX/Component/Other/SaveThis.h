#pragma once

#include "../Component.h"

//自分自身を保存するクラス
class SaveThis : public Component {
public:
    SaveThis(GameObject& gameObject);
    ~SaveThis();
    virtual void finalize() override;

private:
    SaveThis(const SaveThis&) = delete;
    SaveThis& operator=(const SaveThis&) = delete;
};
