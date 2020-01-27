# Notes

## C++ basic

### signed char and unsigned char

+ different from plain `char`, plain `char` is implemented by the compiler. Using it in arithmetic is dangerous and can cause different behaviour under different compilers.

### size_t and unsigned int
>The actual type of size_t is platform-dependent; a common mistake is to assume size_t is the same as unsigned int, which can lead to programming errors, particularly as 64-bit architectures become more prevalent.

### conversion between `string` and `char*`

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

### 函数指针

```c++
 vector<int> (*fptr)( int ) = ...;
```

### using and typedef

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
### malloc & new

+ malloc is **function**. new is an **operator**
+ new delete 会自动执行构造和析构函数，malloc free不会
+ 成功时new会返回对象类型的指针，而malloc返回void *
+ 失败时new会抛出异常，malloc返回NULL

### placement new
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

### move semantics and rvalue reference

+ 简单来说是把一个对象转型成rvalue. static_cast<T&&>(t);
+ `foo(std::move(a))` 会调用 `foo(T&& a)` 而不是 `foo(T& a)` 或者 `foo(T a)`

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

### perfect forwarding

Usage in 参数推倒。

和 `std::move` 一样，`std::forward` 也是一种转型，但`std::move`蕴含被转型的对象即将不被使用或即将被赋予新的值的含义, 而`std::forward`没有这层含义。

因此`std::forward`经常用于参数比如

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

### volatile

**prevent aggresssive optimization**
```cpp
int i = 100;
while (i == 100) ...
```
i can change unexpectedly from outside the program, thus makes it volatile.

### type conversion
在C++中built-in type 转换使用static_cast，type conversion between class 用dynamic_cast，to remove the const 用const_cast，conversion between pointers使用reinterpret_cast。

### (typename) vs. static_cast
`(typename)` is a strong conversion, like `reinterpret_cast`, while static_cast pose some type checks and constraints.

### inline vs. macro

inlining can reduce the costs of function calling stack. it still has type check in function, so safer than macro #define.

### override & final

to detect more errors in compilation about the inheritance of virtual function, to make sure it overrides a virtual function in base class.

`void func() const override` 说明必须重载基类的某个虚函数

`void func() const final` 说明不能再被重载

`class Base {...} final` 说明不能被继承

### emplace_back
在复杂情形中，有时候需要先创建对象在传入`insert`作为参数，带来了额外的开销。
`emplace` 直接从传入的参数中创建对象，进一步优化了开销。

```cpp
// cross your finger so that the optimizer is really good
m.insert(std::make_pair(4, Complicated(anInt, aDouble, aString))); 

// should be easier for the optimizer
m.emplace(4, anInt, aDouble, aString);
```

### lambda 表达式

+ `[&]` 默认引用捕获
+ `[=]` 默认值捕获
+ `[&, i]` 默认引用捕获，i以值捕获
+ `[=, &i]` 默认值捕获，i以引用捕获
+ `[&]( auto x ) -> int { return x;  }`

### decltype & auto

`auto` will be exactly the same as the type of the return value, if a function returns a reference, then the auto will also be reference.

`decltype` will only take the type of the input.

## 操作系统
### 线程与进程的关系
+ cannot execute independently

+ 线程属于进程，并运行在进程空间内，划分尺度小于进程，多个线程共享内存
+ 每个独立的线程有一个程序运行的入口、顺序执行序列和程序的出口。但是线程不能够独立执行，必须依存在应用程序中，由应用程序提供多个线程执行控制
+ 多线程的意义在于一个应用程序中，有多个执行部分可以同时执行
+ 一个应用程序运行后被抽象为一个进程

### heap & stack

+ stack. Allocate when a function calls, free when function returns
+ heap. Dynamically allocate, controlled by the program

### 虚拟内存

应用程序 use 连续的virtual memory space. 但实际上是segments of physical memory space，还有一些储存outside storage上，在需要的时候进行data swap。

### 编译器与解释器

compiler compiles the codes into lower level codes and generate executable which can execute indepently.

interpreter runs the codes, the script will always need an interpreter to run. 

编译器将高级代码编译成低级代码，生成的可执行文件可以独立运行。
解释器直接运行程序，程序总是需要解释器来运行。

### red black tree
+ every node is red or black
+ root is black
+ every leaf is black
+ if a node is red, the twos childs are black
+ every node has same amount of black nodes in their path to their descendant leaves.




