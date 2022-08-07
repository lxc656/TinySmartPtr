//
// Created by 刘晓辰 on 2022/8/2.
//
#ifndef TINYSMARTPTR_SMARTPTR_H
#define TINYSMARTPTR_SMARTPTR_H

#include<utility>
#include"utils.h"

template <typename T>
class smart_ptr {
public:
    template <typename U>
    friend class smart_ptr;

    explicit smart_ptr(T *ptr = nullptr): ptr_(ptr) {
        if(ptr) {
            shared_count_ = new shared_count();
        }
    }

    ~smart_ptr() {
        printf("~smart_ptr(): %p\n", this);
        if(ptr_ && shared_count_->reduce_count()) {
            delete ptr_;
            delete shared_count_;
        }
    }

    template <typename U>
    smart_ptr(const smart_ptr<U>& other) noexcept {
        ptr_ = other.ptr_;
        if(ptr_) {
            other.shared_count_->add_count();
            shared_count_ = other.shared_count_;
        }
    }

    template <typename U>
    smart_ptr(smart_ptr<U>&& other) noexcept {
        ptr_ = other.ptr_;
        if(ptr_) {
            shared_count_ = other.shared_count_;
            other.ptr_ = nullptr;
        }
    }

    template <typename U>
    smart_ptr(const smart_ptr<U>& other, T* ptr) noexcept {
        ptr_ = ptr;
        if(ptr) {
            other.shared_count_->add_count();
            shared_count_ = other.shared_count_;
        }
    }

    void swap(smart_ptr& rhs) {
        using std::swap;
        swap(ptr_, rhs.ptr_);
        swap(shared_count_, rhs.shared_count_);
    }

    smart_ptr& operator=(smart_ptr rhs) noexcept {
        rhs.swap(*this);
        return *this;
    }

    T* get() const {
        return ptr_;
    }

    long use_count() const noexcept {
        if(ptr_) {
            return shared_count_->get_count();
        } else {
            return 0;
        }
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
    shared_count* shared_count_;
};

template <typename T>
void swap(smart_ptr<T>& lhs, smart_ptr<T> rhs) noexcept {
    lhs.swap(rhs);
}

template <typename T, typename U>
smart_ptr<T> static_pointer_cast(const smart_ptr<U>& other) noexcept {
    T* ptr = static_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

template <typename T, typename U>
smart_ptr<T> dynamic_pointer_cast(const smart_ptr<U>& other) noexcept {
    T* ptr = dynamic_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

#endif //TINYSMARTPTR_SMARTPTR_H