# Effective C++


## 可以显示移除复制构造函数 `=deleted`,

## 移动构造函数和移动赋值函数 `foo(int&&)`

## 为多态基类声明 `virtual` 析构函数，保证正确析构

## `rule of three`, 同时修改复制构造，赋值函数`operator==()`和析构函数

## 以独立语句将 new 对象放入智能指针 ,`shared_ptr<>(new int)` 防止内存泄漏

## public 继承是 `is a` 关系

## 声明 pure virtual 函数是为了只继承接口，声明virtual函数是为了继承接口和缺省实现

## 在嵌套从数类型前必须加typename `typename C::iterator iter`
模板声明中 typename 与 class 等价 `template <typename T>` == `template <class T>`

## 模板特化与偏特化

```cpp
// 全特化
template<typename T>
struct my_is_void {
    static const bool value = false;
};

template<>
struct my_is_void<void> {
    static const bool value = true;
};

// 偏特化
template<typename T>
struct my_is_pointer {
    static const bool value = false;
};

template<typename T>
struct my_is_pointer<T*> {
    static const bool value = true;
};

```

用于 `iterator_traits`

  
