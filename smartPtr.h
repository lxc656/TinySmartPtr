//
// Created by 刘晓辰 on 2022/8/2.
//
#ifndef TINYSMARTPTR_SMARTPTR_H
#define TINYSMARTPTR_SMARTPTR_H
template <typename T>
class smart_ptr {
public:
    explicit smart_ptr(T *ptr = nullptr): ptr_(ptr) {}
    ~smart_ptr() {
        delete ptr_;
    }
    T* get() const {
        return ptr_;
    }
    T& operator *() const {
        return *ptr_;
    }
    T* operator ->() const {
        return ptr_;
    }
    operator bool() const {
        return ptr_;
    }
private:
    T* ptr_;
};
#endif //TINYSMARTPTR_SMARTPTR_H