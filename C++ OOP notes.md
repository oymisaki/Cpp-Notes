
# OOP

## virtual, dynamic binding 
每个类使用一个虚函数表，每个类对象用一个虚表指针。如果使用了virtual关键字，程序将根据引用或指针指向的 对 象 类 型 来选择方法，否则使用引用类型或指针类型来选择方法.

when base class a virtual function, the base class was added an `vfptr`.
the memory of base will be like:
```
class base
    vfptr;
    members;
```

And when `Base * p = new Derived()`, the derived class will also generate his own `vfptr`,pointed to his own virtual function table. And `p` will have the derived class's `vfptr`.


## initialization list
`const` member and `reference` member must be initialized in initialization list, because they dont support assignment.

## protected, private, public
derived class can visit protected member of base class, but not the private members.

>**The access modifiers work on class level, and not on object level.**
同一个类的不同对象**可以访问**彼此的**私有成员**，这在拷贝，赋值中常用。

## swap and copy idiom

用`friend swap` 达成`assignment` 的目的
+ 优点，不需要进行`self-assignment check`
+ 缺点，需要声明 `tmp` 或者将参数改为 `传值`，带来效率损失

**move assignment不要用**