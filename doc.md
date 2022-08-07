# 关于拷贝构造函数

最简单的情况显然是禁止拷贝，for example:

```c++
template <typename T>
class smaert_ptr {
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

```