#include "DrawString.h"
#include "../Sprite/Sprite.h"
#include "../Transform/Transform2D.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

DrawString::DrawString() :
    mNumberSprite(nullptr),
    mFontSprite(nullptr),
    mNumberFileName(""),
    mFontFileName("") {
}

DrawString::~DrawString() = default;

void DrawString::initialize() {
    mNumberSprite = std::make_unique<Sprite>(mNumberFileName);
    mFontSprite = std::make_unique<Sprite>(mFontFileName);
}

void DrawString::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["drawString"];
    if (obj.IsObject()) {
        JsonHelper::getString(obj, "number", &mNumberFileName);
        JsonHelper::getString(obj, "font", &mFontFileName);
    }
}

void DrawString::drawAll(const Matrix4 & proj) const {
    for (const auto& param : mParamsInt) {
        drawInt(param, proj);
    }
    for (const auto& param : mParamsFloat) {
        drawFloat(param, proj);
    }
    for (const auto& param : mParamsString) {
        drawString(param, proj);
    }
}

void DrawString::clear() {
    mParamsInt.clear();
    mParamsFloat.clear();
    mParamsString.clear();
}

void DrawString::drawNumber(int number, const Vector2 & position, const Vector2 & scale, const Vector3& color, float alpha, Pivot pivot) {
    ParamInt param;
    param.number = number;
    param.position = position;
    param.scale = scale;
    param.color = color;
    param.alpha = alpha;
    param.pivot = pivot;

    mParamsInt.emplace_back(param);
}

void DrawString::drawNumber(float number, const Vector2 & position, const Vector2 & scale, int decimalDigits, const Vector3& color, float alpha, Pivot pivot) {
    ParamFloat param;
    param.number = number;
    param.position = position;
    param.scale = scale;
    param.decimalDigits = decimalDigits;
    param.color = color;
    param.alpha = alpha;
    param.pivot = pivot;

    mParamsFloat.emplace_back(param);
}

void DrawString::drawString(const std::string & alphabet, const Vector2 & position, const Vector2 & scale, const Vector3 & color, float alpha, Pivot pivot) {
    ParamString param;
    param.alphabet = alphabet;
    param.position = position;
    param.scale = scale;
    param.color = color;
    param.alpha = alpha;
    param.pivot = pivot;

    mParamsString.emplace_back(param);
}

void DrawString::drawInt(const ParamInt & param, const Matrix4 & proj) const {
    //要素取り出し
    auto number = param.number;
    auto pos = param.position;
    auto scale = param.scale;
    auto color = param.color;
    auto alpha = param.alpha;
    auto pivot = param.pivot;

    //マイナスは扱わない
    if (number < 0) {
        number = 0;
    }

    //桁数計算
    int digit = 1;
    for (int i = number; i >= 10; i /= 10) {
        digit++;
    }

    //描画する文字の横幅
    const float OFFSET_X = WIDTH * scale.x;
    //描画する文字列のサイズを計算
    auto size = Vector2(digit * OFFSET_X, HEIGHT * scale.y);
    //ピボットから描画位置を調整
    computePositionFromPivot(&pos, size, pivot);

    //数字を文字列化し、1文字ずつ取り出す
    auto& t = mNumberSprite->transform();
    t.setScale(scale);
    mNumberSprite->setColor(color);
    mNumberSprite->setAlpha(alpha);
    for (const auto& n : std::to_string(number)) {
        t.setPosition(pos);
        //数字のテクスチャが数字1つにつき幅32高さ64
        //文字と文字を引き算し、整数値を取得している
        float num = (n - '0') * WIDTH;
        num /= SPRITE_WIDTH;
        mNumberSprite->setUV(num, 0.f, num + WIDTH_RATE, 1.f);

        //ワールド座標を更新し、描画
        t.computeWorldTransform();
        mNumberSprite->draw(proj);

        //1文字描画したら1桁分右にずらす
        pos.x += OFFSET_X;
    }
}

void DrawString::drawFloat(const ParamFloat & param, const Matrix4 & proj) const {
    //要素取り出し
    auto number = param.number;
    auto pos = param.position;
    auto scale = param.scale;
    auto decimalDigits = param.decimalDigits;
    auto color = param.color;
    auto alpha = param.alpha;
    auto pivot = param.pivot;

    //マイナスは扱わない
    if (number < 0) {
        number = 0;
    }

    //小数部分の桁数指定
    auto num = StringUtil::floatToString(number, decimalDigits);

    //描画する文字の横幅
    const float OFFSET_X = WIDTH * scale.x;
    const float OFFSET_PERIOD_X = PERIOD_WIDTH * scale.x;
    //描画する文字列のサイズを計算
    auto size = Vector2((num.length() - 1) * OFFSET_X + OFFSET_PERIOD_X, HEIGHT * scale.y);
    //ピボットから描画位置を調整
    computePositionFromPivot(&pos, size, pivot);

    //数字を文字列化し、1文字ずつ取り出す
    auto& t = mNumberSprite->transform();
    t.setScale(scale);
    mNumberSprite->setColor(color);
    mNumberSprite->setAlpha(alpha);
    for (const auto& n : num) {
        t.setPosition(pos);
        //数字のテクスチャが数字1つにつき幅32高さ64
        //文字と文字を引き算し、整数値を取得している
        float offsetX = 0.f;
        if (n == '.') {
            constexpr float num = 10 * WIDTH_RATE; //ピリオドは画像の10番目
            mNumberSprite->setUV(num, 0.f, num + PERIOD_RATE, 1.f);

            //「.」のときは1文字の半分ずらす
            offsetX = PERIOD_WIDTH;
        } else {
            float num = (n - '0') * WIDTH;
            num /= SPRITE_WIDTH;
            mNumberSprite->setUV(num, 0.f, num + WIDTH_RATE, 1.f);

            //1文字描画したら1桁分右にずらす
            offsetX = OFFSET_X;
        }

        //ワールド座標を更新し、描画
        t.computeWorldTransform();
        mNumberSprite->draw(proj);

        pos.x += offsetX;
    }
}

void DrawString::drawString(const ParamString & param, const Matrix4 & proj) const {
    //要素取り出し
    auto alphabet = param.alphabet;
    auto pos = param.position;
    auto scale = param.scale;
    auto color = param.color;
    auto alpha = param.alpha;
    auto pivot = param.pivot;

    //描画する文字の横幅
    const float OFFSET_X = WIDTH * scale.x;
    //描画する文字列のサイズを計算
    auto size = Vector2(alphabet.length() * OFFSET_X, HEIGHT * scale.y);
    //ピボットから描画位置を調整
    computePositionFromPivot(&pos, size, pivot);

    auto& trans = mFontSprite->transform();
    trans.setScale(scale);
    mFontSprite->setColor(color);
    mFontSprite->setAlpha(alpha);
    for (const auto& c : alphabet) {
        trans.setPosition(pos);

        int t = static_cast<int>(c);
        t = Math::clamp<int>(t, 32, 127);
        t -= 32;

        float left = t % WIDTH_CHAR_COUNT;
        left /= WIDTH_CHAR_COUNT;
        float top = t / WIDTH_CHAR_COUNT;
        top /= HEIGHT_CHAR_COUNT;
        mFontSprite->setUV(left, top, left + WIDTH_RATE, top + FONT_HEIGHT_RATE);

        //ワールド座標を更新し、描画
        trans.computeWorldTransform();
        mFontSprite->draw(proj);

        //描画位置を1文字分ずらす
        pos.x += OFFSET_X;
    }
}

void DrawString::computePositionFromPivot(Vector2 * pos, const Vector2 & size, Pivot pivot) const {
    switch (pivot) {
    case Pivot::CENTER_TOP:
        pos->x -= size.x / 2.f;
        break;
    case Pivot::RIGHT_TOP:
        pos->x -= size.x;
        break;
    case Pivot::CENTER_LEFT:
        pos->y -= size.y / 2.f;
        break;
    case Pivot::CENTER:
        *pos -= size / 2.f;
        break;
    case Pivot::CENTER_RIGHT:
        pos->x -= size.x;
        pos->y -= size.y / 2.f;
        break;
    case Pivot::LEFT_BOTTOM:
        pos->y -= size.y;
        break;
    case Pivot::CETNER_BOTTOM:
        pos->x -= size.x / 2.f;
        pos->y -= size.y;
        break;
    case Pivot::RIGHT_BOTTOM:
        *pos -= size;
        break;
    default:
        break;
    }
}
