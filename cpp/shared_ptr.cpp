template <typename T>
class mySmartPtr
{
public:
    // 构造函数 默认为空
    mySmartPtr(): pointer(0), counter(0)
    {
    } 
 
    // 形参为指针的构造函数
    mySmartPtr(T* p): pointer(0), counter(0){
        *this = p;
    }
 
    // 复制构造函数                                               
    mySmartPtr(const mySmartPtr<T> &p): 
    pointer(p.pointer), counter(p.counter){
        Increase();
    }
 
    ~mySmartPtr(){
        Decrease();
    }
 
    // 指针的赋值操作符，类型不同，不是自赋值
    mySmartPtr operator=(T* p){
        Decrease();
        if (p){
            pointer = p;
            counter = new int(1);
        }
        else {
            pointer = 0;
            counter = 0;
        }
        return *this;
    }
 
    // 智能指针赋值操作符
    mySmartPtr operator=(mySmartPtr<T> &p){
        // 处理自赋值
        if (this != &p){
            Decrease();
            pointer = p.pointer;
            counter = p.counter;
            Increase();
        }
        return *this;
    }
 
    operator bool() const{
        return counter != 0;
    }
 
    // ×操作符重载
    T* operator*() const{
        return this;
    }
 
    // ->操作符重载
    T* operator->() const{
        return pointer;
    }
 
    // 返回底层指针
    int getPtrPointer() const{
        return *pointer;
    }
 
    // 返回引用计数
    int getPtrCounter() const{
        return *counter;
    }
 
private:
    T*      pointer;
    int*    counter;
 
    void Increase(){
        if (counter) 
            ++*counter;
    }
 
    void Decrease(){
        if (counter && --*counter == 0){
            delete pointer;
            delete counter;
            counter = 0;
            pointer = 0;
        }
    }
 
};
