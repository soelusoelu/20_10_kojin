#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

class Component;

class ComponentManager {
    using ComponentPtr = std::shared_ptr<Component>;
    using ComponentPtrList = std::list<ComponentPtr>;

public:
    ComponentManager();
    ~ComponentManager();
    //各コンポーネントのstartを一度だけ実行
    void start();
    //所有するすべてのコンポーネントを更新
    void update() const;
    //所有するすべてのコンポーネントを遅延更新
    void lateUpdate() const;
    //所有するすべてのコンポーネントの終了処理を実行
    void finalize();
    //コンポーネントの追加
    void addComponent(const ComponentPtr& component);

    //所有するすべてのコンポーネントのonUpdateWorldTransformを実行
    void onUpdateWorldTransform() const;
    //所有するすべてのコンポーネントのonSetActiveを実行
    void onEnable(bool value) const;

    //全コンポーネントの取得
    const ComponentPtrList& getAllComponents() const;

    //コンポーネントの取得
    template<typename T>
    std::shared_ptr<T> getComponent() const {
        std::shared_ptr<T> comp = nullptr;
        for (const auto& c : mStartComponents) {
            comp = std::dynamic_pointer_cast<T>(c);
            if (comp) {
                return comp;
            }
        }
        for (const auto& c : mComponents) {
            comp = std::dynamic_pointer_cast<T>(c);
            if (comp) {
                return comp;
            }
        }
        //最後まで見つからなければnullptrを返す
        return comp;
    }

    //指定した型のコンポーネントをすべて取得
    template<typename T>
    std::vector<std::shared_ptr<T>> getComponents() const {
        std::vector<std::shared_ptr<T>> components;
        for (const auto& c : mStartComponents) {
            auto comp = std::dynamic_pointer_cast<T>(c);
            if (comp) {
                components.emplace_back(comp);
            }
        }
        for (const auto& c : mComponents) {
            auto comp = std::dynamic_pointer_cast<T>(c);
            if (comp) {
                components.emplace_back(comp);
            }
        }
        return components;
    }

private:
    ComponentManager(const ComponentManager&) = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;

private:
    ComponentPtrList mStartComponents;
    ComponentPtrList mComponents;
};
