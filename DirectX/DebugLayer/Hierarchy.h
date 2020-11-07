#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <utility>

class DrawString;
class GameObject;
class Button;

class Hierarchy {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;

public:
    Hierarchy(DrawString* drawString);
    ~Hierarchy();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void update();
    void setGameObjectToButton(const GameObjectPtrList& gameObjects);
    //マネージャーに登録されてる全ゲームオブジェクトを表示
    void drawGameObjects() const;

private:
    DrawString* mDrawString;
    std::list<std::pair<std::unique_ptr<Button>, GameObjectPtr>> mButtons;
    //画面に表示する行数
    int mNumRowsToDisplay;
    //行間
    float mLineSpace;
    //Inspectorの位置
    float mInspectorPositionX;
    //表示する位置
    Vector2 mPosition;
    //文字のスケール
    Vector2 mScale;
    //表示位置をずらす文字数
    int mOffsetCharCountX;
    int mOffsetCharCountY;
    //1文字の大きさ
    float mCharWidth;
    float mCharHeight;
    //非アクティブ時の文字の透過度
    float mNonActiveAlpha;
};
