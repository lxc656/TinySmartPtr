#include <iostream>
#include"smartPtr.h"
#include "utils.h"

//用于进行测试
class shape {
public:
    virtual ~shape() {}
};

class circle:public shape {
public:
    ~circle() {
        puts("~circle()");
    }
};
int main() {
    std::cout << "Test Started" << std::endl;
    smart_ptr<circle> ptr1(new circle());
    printf("use count of ptr1 is %ld\n", ptr1.use_count());
    smart_ptr<shape> ptr2;
    printf("use count of ptr2 is %ld\n", ptr2.use_count());
    ptr2 = ptr1;
    printf("use count of ptr2 is %ld\n", ptr2.use_count());
    if(ptr1) {
        puts("ptr1 is not empty");
    }
    smart_ptr<circle> ptr3 = dynamic_pointer_cast<circle>(ptr2);
    printf("use count of ptr3 is %ld\n", ptr3.use_count());
    return 0;
}
