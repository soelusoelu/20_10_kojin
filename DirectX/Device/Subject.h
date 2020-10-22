#pragma once

#include <list>
#include <functional>

class Subject {
public:
    Subject();
    ~Subject();
    void addObserver(const std::function<void()>& observer);
    void notify() const;

private:
    std::list<std::function<void()>> mObservers;
};
