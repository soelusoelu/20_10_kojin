#include "Subject.h"

Subject::Subject() = default;
Subject::~Subject() = default;

void Subject::addObserver(const std::function<void()>& observer) {
    mObservers.emplace_back(observer);
}

void Subject::notify() const {
    for (const auto& observer : mObservers) {
        observer();
    }
}
