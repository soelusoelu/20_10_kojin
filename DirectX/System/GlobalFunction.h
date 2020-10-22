#pragma once

template <typename T>
inline void safeDelete(T*& p) {
    delete p;
    p = nullptr;
}

template <typename T>
inline void safeDeleteArray(T*& p) {
    delete[] p;
    p = nullptr;
}

template <typename T>
inline void safeRelease(T*& p) {
    if (p) {
        p->Release();
        p = nullptr;
    }
}
