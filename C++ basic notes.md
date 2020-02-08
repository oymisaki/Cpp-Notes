# Notes

## signed char and unsigned char

+ different from plain `char`, plain `char` is implemented by the compiler. Using it in arithmetic is dangerous and can cause different behaviour under different compilers.

## size_t and unsigned int
>The actual type of size_t is platform-dependent; a common mistake is to assume size_t is the same as unsigned int, which can lead to programming errors, particularly as 64-bit architectures become more prevalent.

## conversion between `string` and `char*`

+ `char*` -> `string`. 直接赋值
+ `string` -> `char*`.
  ```cpp
    str.c_str(); // return a const char *, immutable

    // modern c++
    std::vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);

    copy(cstr.begin(), cstr.end(), ostream_iterator<char>(cout));
  ```

## print type

```cpp
#include<typeinfo>

int main()
{
  int a = 1;
  cout << typeid(a).name() << endl;
}
```

## 函数指针

```c++
 vector<int> (*fptr)( int ) = ...;
```

## using and typedef

+ `typedef void (*FP) (int, const std::string&);`
+ `using FP = void (*) (int, const std::string&);`
+ using 可以用于 模板别名， template alias
  ```c++
  template <typename T>
  using Vec = MyVector<T, MyAlloc<T>>;
   
  // usage
  // typedef can not do this
  Vec<int> vec;
  ```
## malloc & new

+ malloc is **function**. new is an **operator**
+ new delete 会自动执行构造和析构函数，malloc free不会
+ 成功时new会返回对象类型的指针，而malloc返回void *
+ 失败时new会抛出异常，malloc返回NULL

## placement new
不分配新内存，只构造对象的new
```cpp
char memory[sizeof(Fred)];     // Line #1
void* place = memory;          // Line #2
Fred* f = new(place) Fred();
```

`uninitialized_copy` is a range version of this code.
more precisely, when combined with `make_move_iterator`
```cpp
uninitialized_copy(make_move_iterator(first),
                   make_move_iterator(last),
                   dst);
// the same as
// which means will call the move construction
Fred* f = new(place) Fred(std::move(*iter));
```

```cpp
T a;
new (p) T{a};
```
必须手动调用析构释放. `p->~Fred()`, 这也是唯一需要手动调用析构函数的情况。

## move semantics and rvalue reference

+ 简单来说是把一个对象转型成rvalue. static_cast<T&&>(t);
+ `foo(std::move(a))` 会调用 `foo(T&& a)` 而不是 `foo(T& a)` 或者 `foo(T a)`
+ **注意std::move只是indicate it may be moved from，并不会internally销毁目标**
+ So set the moved resource to default state is something move constructors should do

不用deep copy,直接把右值的指针搬过来，把右值的指针置为null
```cpp
  string(string&& that)   // string&& is an rvalue reference to a string
  {
      data = that.data;
      that.data = nullptr;
  }

  // usage 
  string(std::move(str));
```

Usage in `swap`, 不需要copy
```cpp
template <class T>
swap(T& a, T& b) {
    T tmp(std::move(a)); // T(&&)
    a = std::move(b);   // operator=(&&)
    b = std::move(tmp); // operator=(&&)
}
```

Usage in `unique_ptr`
```cpp
del = std::move(right_value.del);
```

## perfect forwarding

Usage in 参数推倒。

Parameter forward example. 现在要把 `wrapper` 的参数传到 `func`

```cpp
template <typename T1, typename T2>
void wrapper(T1 e1, T2 e2) {
    func(e1, e2);
} // 那么当func参数为引用时，传值会出错

template <typename T1, typename T2>
void wrapper(T1& e1, T2& e2) {
    func(e1, e2);
} // 当给wrapper传入rvalue时报错

template <typename T1, typename T2>
void wrapper(const T1& e1, const T2& e2) {
    func(e1, e2);
} // 当给func需要修改e1时报错

// 唯一的解决办法是写所有可能的模板
```

`C++ 11` 引入了新的类型推倒type deduction，当`&&`遇到`&`总是`&`胜出。
```cpp
template <class T>
void func(T&& t) {
} 
```
在这个模板里的 `T&&` 不是类型引用，而是告诉编译器怎么做`type deduction`。
结合`lvalue`和`rvalue`，当传入`lvalue`时，参数解读类型为`T& && = T&`，`rvalue`则为
`T&& && = T&&`

结合**variadic templates**:
```cpp
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

成功做到了，右值传值，左值传引用。


## Smart Pointers

**实现智能指针应该注意的点**
+ explicit 构造
+ explicit bool转型
+ noexcept
+ swap函数
+ 基类子类指针之间的赋值和构造
+ 在构造失败时释放资源
+ const
+ 移动构造


**关于智能指针的线程安全性**
+ 智能指针内部保证析构和引用计数是线程安全的，比如`shared_ptr`引用计数的自增自减都是原子操作，
析构函数只调用一次。`weak_ptr.lock`等都是原子操作。
+ 但不保证对其指向的数据块的线程安全性

### unique_ptr
+ `release()` 解除控制内存的权利和义务，返回`raw pointer`
+ `reset()` 释放内存

### weak_ptr
+ `lock()` 返回等价于 `expired() ? shared_ptr<T>() : shared_ptr<T>(*this)`，因此可能返回空对象
+ `expired()` 是否引用计数为0, 返回 `true or false`

### do not use get() in shared_ptr

```cpp
shared_ptr<B> sp (new D);   //通过隐式转换，储存D的指针。
B* b = sp.get();           //shared_ptr辛辛苦苦隐藏的原生指针就这么被刨出来了。
D* d = dynamic_cast<D*>(b); //这是使用get的正当理由吗？

// 正确的做法
shared_ptr<B> spb (new D) ;
shared_ptr<D> spd =shared_dynamic_cast<D>(spb);//变成子类的指针
```

### weak_ptr and shared_ptr
`shared_ptr` 是强引用，每次复制会增加引用计数。`weak_ptr` 是弱引用，使用它本身不会增加计数，只有当其他某处有 `shared_ptr`
指向其资源时，`weak_ptr`才会返回一个非空的`shared_ptr`.

>This is exactly what a weak pointer does -- it allows you to locate an object if it's still around, but doesn't keep it around if nothing else needs it.

```cpp
#include <iostream>
#include <memory>
 
std::weak_ptr<int> gw;
 
void f()
{
    if (auto spt = gw.lock()) { // 使用之前必须复制到 shared_ptr
      std::cout << *spt << "\n";
    }
    else {
      std::cout << "gw is expired\n";
    }
}
 
int main()
{
  // output 
  // 42
  // gw is expired
    {
      auto sp = std::make_shared<int>(42);
	    gw = sp;
	    f();
    }
    f();
}
```

使用智能指针也可能造成内存泄漏。调用顺序可能是 `new`, `g()`, `g()` 异常时，内存就会泄漏。
```cpp
function ( shared_ptr<int>(new int), g( ) ); 
```

### A basic use of weak_ptr together with shared_ptr
倘若树的节点，同时包含指向子节点的指针和父节点的指针，如果都用`shared_ptr`，会出现循环引用的问题。
因此可以把指向父节点的指针声明成 `weak_ptr`

### shared_ptr and make_shared

>std::shared_ptr manages two entities:
>the control block (stores meta data such as ref-counts, type-erased deleter, etc)
>the object being managed
>std::make_shared performs a single heap-allocation accounting for the space necessary for both the control block and the data. In the other case, new Obj("foo") invokes a heap-allocation for the managed data and the std::shared_ptr constructor performs another one for the control block.

Moreover, `make_shared` is exception-safe as we dont need to use `new` at all.

### shared_ptr & Deleter 

Usage of deleter
```cpp
template<class T>
struct Array_Deleter
{   
    void operator ()( T*){   delete[] p;  }
};

int* pint = new int[100];
shared_ptr<int> p (pint, Array_Deleter<int>());
```
在当使用new开辟的数组时，使用deleter是必须的，也可以用`lambda表达式`写成
```cpp
shared_ptr<int> pp(pint, [](int* p) {delete[] p; });
```

## C++ 11 keyword
### NULL and nullptr

`nullptr` 不能转型成 `int`

```cpp
// C++ program to demonstrate problem with NULL 
#include <bits/stdc++.h> 
using namespace std; 
// function with integer argument 
int fun(int N)   { cout << "fun(int)"; } 
// Overloaded function with char pointer argument 
int fun(char* s)  { cout << "fun(char *)"; } 
int main()  
{ 
    // Ideally, it should have called fun(char *), 
    // but it causes compiler error. 
    fun(NULL);   
} 
```

**more on `nullptr`**
We can **safely delete a nullptr** and it will not cause any exception.

But delete a pointer does not mean setting it to nullptr 
```cpp
int * p = new int(1);
delete p;
delete p; // 0x11111111 some address, undefined behavior
cout << p; // 

p = nullptr;
delete p; // safe
```


### volatile

**prevent aggresssive optimization**
```cpp
int i = 100;
while (i == 100) ...
```
i can change unexpectedly from outside the program, thus makes it volatile.

### override & final

to detect more errors in compilation about the inheritance of virtual function, to make sure it overrides a virtual function in base class.

`void func() const override` 说明必须重载基类的某个虚函数

`void func() const final` 说明不能再被重载

`class Base {...} final` 说明不能被继承

### const & mutable

M&M rule. `mutable` and `mutex` go together. 
简单来说，mutable修饰的object可以在const修饰的成员函数中被修改，体现出一种物理上可被修改，但逻辑上不变的特性。
典型的例子是 `mutex`

```cpp
class ThreadsafeCounter {
  mutable std::mutex m; // The "M&M rule": mutable and mutex go together
  int data = 0;
 public:
  int get() const {
    std::lock_guard<std::mutex> lk(m);
    return data;
  }
  void inc() {
    std::lock_guard<std::mutex> lk(m);
    ++data;
  }
};
```

### lambda 表达式

+ `[&]` 默认引用捕获
+ `[=]` 默认值捕获
+ `[&, i]` 默认引用捕获，i以值捕获
+ `[=, &i]` 默认值捕获，i以引用捕获
+ `[&]( auto x ) -> int { return x;  }`

**使用lambda表达式一定要注意不要瞎传引用, 特别是函数里的局部变量**
```cpp
  void add_event(int t, function<void()> e)
  {
      auto f = [=] {
          std::this_thread::sleep_for(chrono::seconds(t));
          cout << t << endl;
          e();
      };

      events.emplace_back(t, f);
  }
```
上述代码中，`e` 和 `t` 都是局部变量，如果设置为引用，则会出现`undefined behaviour`


### decltype & auto

`decltype` will be exactly the same as the type of the return value, if a function returns a reference, then the auto will also be reference.

`auto` will only take the type of the input, need `auto&` to declare reference.

### noexcept
**允许编译器做出更多更好的优化**
鼓励在以下函数中使用
+ move constructor
+ move assignment
+ destructor. 编译器默认  

**Why move constructors need nonexcept?**
当 `copy constuctor` 出现异常，可以保证源对象不变，新对象没有创造，而 `move constructor` 异常可能会使得源对象变化，从而带来不确定性，因此在 `vector` 等容器中使用 `resize()` 等函数时，涉及到创建一片新的内存空间，并将旧的数据复制过去，此时使用 `move constructor` 会更快一点，但 `std::vector`会确认对象的 `move constrcutor` 是 `noexcept` 的，否则出于安全考虑，他还是会调用 `copy constructor`，即便效率更低。 

## type conversion
type conversion between class 用dynamic_cast，to remove the const 用const_cast，conversion between pointers使用reinterpret_cast。

+ `static_cast`，不会在runtime检查，使用时就知道会传入什么
+ `dynamic_cast`, 会在runtime检查有没有关系，并且在`down-cast`时不支持没有虚函数的基类
+ `(typename)` 只用于`built-in`类型，是将`static_cast`, `reinterpret_cast`结合到一起的

## inline vs. macro

inlining can reduce the costs of function calling stack. it still has type check in function, so safer than macro #define.

## emplace_back
在复杂情形中，有时候需要先创建对象在传入`insert`作为参数，带来了额外的开销。
`emplace` 直接从传入的参数中创建对象，进一步优化了开销。

```cpp
// cross your finger so that the optimizer is really good
m.insert(std::make_pair(4, Complicated(anInt, aDouble, aString))); 

// should be easier for the optimizer
m.emplace(4, anInt, aDouble, aString);
```



# 操作系统


## heap & stack

+ stack. Allocate when a function calls, free when function returns
+ heap. Dynamically allocate, controlled by the program

## 虚拟内存

应用程序 use 连续的virtual memory space. 但实际上是segments of physical memory space，还有一些储存outside storage上，在需要的时候进行data swap。

## 编译器与解释器

compiler compiles the codes into lower level codes and generate executable which can execute indepently.

interpreter runs the codes, the script will always need an interpreter to run. 

编译器将高级代码编译成低级代码，生成的可执行文件可以独立运行。
解释器直接运行程序，程序总是需要解释器来运行。

## red black tree
+ every node is red or black
+ root is black
+ every leaf is black
+ if a node is red, the twos childs are black
+ every node has same amount of black nodes in their path to their descendant leaves.

## Cache locality.
分为
+ temporal locality
+ spatial locality
多级存储下，连续的内存，访问更快。

## 字节对齐
**偏移量是子对象字节数的倍数**



