#pragma once

#include "../Component.h"
#include "../../Sound/3D/Sound3DCone.h"
#include "../../Sound/3D/Listener/Sound3DListenerParam.h"
#include <memory>

class Sound3DListener;

class ListenerComponent : public Component {
public:
    ListenerComponent(GameObject& gameObject);
    ~ListenerComponent();
    virtual void awake() override;
    virtual void update() override;
    //コーンを設定する
    void setCone(Sound3DCone* cone);
    //リスナー構造体を返す
    const Sound3DListenerParam& getListener() const;

private:
    std::shared_ptr<Sound3DListener> mListener;
};
