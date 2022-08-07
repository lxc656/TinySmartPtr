# 关于拷贝构造函数

最简单的情况显然是禁止拷贝，for example:

```c++
template <typename T>
class smart_ptr {
    ...
    smart_ptr(const smart_ptr&) = delete;
    smart_ptr& operator=(const smart_ptr&) = delete;
    ...
}
```


那么是否可以考虑在拷贝智能指针时把对象拷贝一份?
不行，通常人们不会这么用，因为使用智能指针的目的就是要减少对象的拷贝。
并且如果指针类型是指向基类的指针，实际指向的却应该是子类的对象，
在C++里并没有通用的方法可以通过基类的指针来构造出一个子类的对象来

因此为了实现智能指针，可以拷贝时转移指针的所有权，如下所示：

```c++
template <typename T>
class smart_ptr {
    ...
    T* release() {
        T* ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }
    
    //rhs means right-hand side
    void swap(smart_ptr& rhs) {
        using std::swap;
        swap(ptr_, rhs.ptr_);
    }
    ...
    smart_ptr(smart_ptr& other) {
        ptr_ = other.release();
    }
    
    smart_ptr& operator=(smart_ptr &rhs) {
        smart_ptr(rhs).swap(*this);
        return *this;
    }
    ...
};
```
在拷贝构造函数中通过调用`other`的`release`方法来释放它对指针的所有权，
在赋值函数中通过拷贝构造产生一个临时对象并调用`swap`来交换对指针的所有权

赋值函数的实现分为拷贝构造和交换两步，这样保证了异常安全，异常只会在第一步发生，
也就是拷贝构造的过程，这个过程中发生异常对this指针指向的对象不会造成影响，
因此不会出现因赋值导致当前对象被破坏的情况

这便是C++98当中对auto_ptr的实现，但是这样也会有一个问题，一旦把某个smart_ptr
通过赋值传递给另一个smart_ptr，那么它就不再拥有它之前所指向的对象

# 通过移动构造进一步改善

如下是C++11中的unique_ptr对C++98中auto_ptr所做的改进中的一部分：

```c++
template <typename T>
class smart_ptr {
    ...
    smart_ptr(smart_ptr&& other) {
        ptr_ = other.release();
    }
    
    smart_ptr& operator=(smart_ptr rhs) {
        rhs.swap(*this);
        return *this;
    }
    ...
};
```
把拷贝构造函数中的参数类型 `smart_ptr&` 改成了 `smart_ptr&&`，
现在它成了移动构造函数，把赋值函数中的参数类型`smart_ptr&`改成了`smart_ptr`，
在构造参数时直接生成新的智能指针，从而不再需要在函数体中构造临时对象，
现在赋值函数的行为是移动还是拷贝，完全依赖于构造参数时走的是移动构造还是拷贝构造 ，
根据C++的规则，如果提供了移动构造函数而没有手动提供拷贝构造函数，那后者自动被禁用

因此，
（我们假设shape是一个基类的名字）
`smart_ptr<shape> ptr2{ptr1};`会编译出错，因为拷贝构造函数被禁用了，
`smart_ptr<shape> ptr3; ptr3 = ptr2;`也会编译出错，
因为构造=运算符的参数的时候无法通过ptr2这个左值来进行拷贝构造，
只能通过右值移动构造，因此
`smart_ptr<shape> ptr4{std::move(ptr3)}`和
`ptr3 = std::move(ptr1)`是合法的

# 实现从子类指针到基类指针的转换
假设`circle`, `triangle`是`shape`的子类，那么`smart_ptr<circle>`还没有办法自动转换成
`smart_ptr<shape>`，但是在C++当中，`circle*`是可以隐式转换成`shape*`的

我们可以通过修改移动构造函数来实现这个功能，
```c++
template <typename U>
smart_ptr(smart_ptr<U>&& other) {
    ptr_ = other.release();
}
```
`smart_ptr<circle>`可以移动给`smart_ptr<shape>`，
但不能移动给`smart_ptr<triangle>`, 不正确的转换会在编译时直接报错

# 实现shared_ptr

unique_ptr是较安全的智能指针，一个对象只能被单个unique_ptr所拥有，
但是在某些使用场景下也会有多个智能指针同时拥有一个对象这样的需求，这便是
shared_ptr，实现shared_ptr的核心在于要维护一个引用计数，
多个智能指针在共享同一个对象时也要共享同一个引用计数，智能指针析构时
会让引用计数自减，引用计数为0时就调用所指向对象的析构函数

具体实现在`smartPtr.h`的`shared_count`类当中（这个Tiny版本的智能指针
目前还没有采用线程安全的实现方法🤣）