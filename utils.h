//
// Created by 刘晓辰 on 2022/8/7.
//

#ifndef TINYSMARTPTR_UTILS_H
#define TINYSMARTPTR_UTILS_H
#include "smartPtr.h"
class shared_count {
public:
    shared_count() noexcept: count_(1) {}
    void add_count() noexcept {
        ++count_;
    }
    long reduce_count() noexcept {
        return --count_;
    }
    long get_count() const noexcept {
        return count_;
    }
private:
    long count_;
};
#endif //TINYSMARTPTR_UTILS_H
