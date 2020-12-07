#include "SpriteComponent.h"
#include "../../DebugLayer/ImGuiWrapper.h"
#include "../../GameObject/GameObject.h"
#include "../../Imgui/imgui.h"
#include "../../Sprite/Sprite.h"
#include "../../Sprite/SpriteManager.h"
#include "../../Transform/Pivot.h"
#include "../../Transform/Transform2D.h"
#include "../../Utility/LevelLoader.h"

SpriteComponent::SpriteComponent(GameObject& gameObject) :
    Component(gameObject),
    mDrawOrder(0),
    mSprite(std::make_unique<Sprite>()) {
}

SpriteComponent::~SpriteComponent() = default;

void SpriteComponent::awake() {
    addToManager();
    setActive(gameObject().getActive());
}

void SpriteComponent::lateUpdate() {
    mSprite->computeWorldTransform();
}

void SpriteComponent::finalize() {
    mSprite->destroy();
}

void SpriteComponent::onEnable(bool value) {
    setActive(value);
}

void SpriteComponent::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "drawOrder", &mDrawOrder);
    std::string str;
    if (JsonHelper::getString(inObj, "fileName", &str)) {
        setTextureFromFileName(str);
    }
    bool isActive = true;
    if (JsonHelper::getBool(inObj, "isActive", &isActive)) {
        setActive(isActive);
    }
    Vector2 vec2;
    if (JsonHelper::getVector2(inObj, "position", &vec2)) {
        transform().setPosition(vec2);
    }
    float value;
    if (JsonHelper::getFloat(inObj, "rotation", &value)) {
        transform().setRotation(value);
    }
    if (JsonHelper::getVector2(inObj, "scale", &vec2)) {
        transform().setScale(vec2);
    }
    Vector3 vec3;
    if (JsonHelper::getVector3(inObj, "color", &vec3)) {
        setColor(vec3);
    }
    if (JsonHelper::getFloat(inObj, "alpha", &value)) {
        setAlpha(value);
    }
    Vector4 vec4;
    if (JsonHelper::getVector4(inObj, "uv", &vec4)) {
        setUV(vec4.x, vec4.y, vec4.z, vec4.w);
    }
    if (JsonHelper::getString(inObj, "pivot", &str)) {
        Pivot pivot = Pivot::NONE;
        PivotFunc::stringToPivot(str, &pivot);
        transform().setPivot(pivot);
    }
}

void SpriteComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setInt(alloc, inObj, "drawOrder", mDrawOrder);
    JsonHelper::setString(alloc, inObj, "fileName", fileName());
    JsonHelper::setBool(alloc, inObj, "isActive", getActive());
    JsonHelper::setVector2(alloc, inObj, "position", transform().getPosition());
    JsonHelper::setFloat(alloc, inObj, "rotation", transform().getRotation());
    JsonHelper::setVector2(alloc, inObj, "scale", transform().getScale());
    JsonHelper::setVector3(alloc, inObj, "color", getColor());
    JsonHelper::setFloat(alloc, inObj, "alpha", getAlpha());
    JsonHelper::setVector4(alloc, inObj, "uv", getUV());
    std::string dst;
    PivotFunc::pivotToString(transform().getPivotEnum(), dst);
    JsonHelper::setString(alloc, inObj, "pivot", dst);
}

void SpriteComponent::drawInspector() {
    ImGui::Text("FileName: %s", fileName().c_str());
    transform().drawInspector();

    auto color = getColor();
    if (ImGuiWrapper::colorEdit3("Color", color)) {
        setColor(color);
    }

    auto alpha = getAlpha();
    if (ImGuiWrapper::sliderFloat("Alpha", alpha, 0.f, 1.f)) {
        setAlpha(alpha);
    }

    auto uv = getUV();
    if (ImGuiWrapper::sliderVector4("UV", uv, 0.f, 1.f)) {
        setUV(uv.x, uv.y, uv.z, uv.w);
    }
}

void SpriteComponent::draw(const Matrix4& proj) const {
    mSprite->draw(proj);
}

Transform2D& SpriteComponent::transform() const {
    return mSprite->transform();
}

void SpriteComponent::setColor(const Vector3& color) {
    mSprite->setColor(color);
}

void SpriteComponent::setColor(float r, float g, float b) {
    mSprite->setColor(r, g, b);
}

const Vector3& SpriteComponent::getColor() const {
    return mSprite->getColor();
}

void SpriteComponent::setAlpha(float alpha) {
    mSprite->setAlpha(alpha);
}

float SpriteComponent::getAlpha() const {
    return mSprite->getAlpha();
}

void SpriteComponent::setUV(float l, float t, float r, float b) {
    mSprite->setUV(l, t, r, b);
}

const Vector4& SpriteComponent::getUV() const {
    return mSprite->getUV();
}

const Vector2& SpriteComponent::getTextureSize() const {
    return mSprite->getTextureSize();
}

void SpriteComponent::setActive(bool value) {
    mSprite->setActive(value);
}

bool SpriteComponent::getActive() const {
    return mSprite->getActive();
}

bool SpriteComponent::isDead() const {
    return mSprite->isDead();
}

void SpriteComponent::setTextureFromFileName(const std::string& fileName) {
    mSprite->setTextureFromFileName(fileName);
}

void SpriteComponent::setTexture(const std::shared_ptr<Texture>& texture) {
    mSprite->setTexture(texture);
}

const Texture& SpriteComponent::texture() const {
    return mSprite->texture();
}

const Shader& SpriteComponent::shader() const {
    return mSprite->shader();
}

const std::string& SpriteComponent::fileName() const {
    return mSprite->fileName();
}

int SpriteComponent::getDrawOrder() const {
    return mDrawOrder;
}

void SpriteComponent::setSpriteManager(SpriteManager* manager) {
    mSpriteManager = manager;
}

void SpriteComponent::addToManager() {
    if (mSpriteManager) {
        mSpriteManager->addComponent(shared_from_this());
    }
}
