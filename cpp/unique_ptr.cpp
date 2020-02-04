#ifndef UNIQUE_PTR
#define UNIQUE_PTR

class Delete
{
public:
    template <typename T>
    void operator()(T *p) const
    {
        delete p;
    }
};

template <typename T, typename D = Delete>
class unique_ptr
{
private:
    T *un_ptr = nullptr;
    D del;
    /* data */
public:
    explicit unique_ptr(T *pp = nullptr, const D &dd = D())
        : un_ptr(pp), del(dd) {}
    ~unique_ptr()
    {
        if(un_ptr)
            del(un_ptr);
    }

    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;

    // 右值引用 c++ 11
    // 重载右值引用拷贝构造函数
    // 注意noexcept
    unique_ptr(unique_ptr &&rvalue) noexcept
    : un_ptr(right_value.un_ptr), del(std::move(right_value.del))
    {
        right_value.un_ptr = nullptr;
    }

    // 赋值
    // move constructor and swap idiom
    unique_ptr &operator=(unique_ptr &&right_value) noexcept
    {
        if (this != &right_value)
        {
            if(un_ptr)
                del(un_ptr);
            un_ptr = right_value.un_ptr;

            // 不是复制而是直接拿取
            del = std::move(right_value.del);
            right_value.un_ptr = nullptr;
        }
        return *this;
    }

    // 放弃控制权
    T *release()
    {
        T *tmp = un_ptr;
        un_ptr = nullptr;
        return tmp;
    }

    // 释放资源
    void reset()
    {
        if (un_ptr)
            del(un_ptr);
    }

    void reset(T *q)
    {
        if (un_ptr != q)
        {
            if (un_ptr)
                del(un_ptr);
            un_ptr = q;
        }
    }

    void swap(unique_ptr &other) noexcept
    {
        using std::swap;
        swap(un_ptr, other.un_ptr);
        swap(del, other.del);
    }

    T *get() { return un_ptr; }
    D &get_deleter() { return del; }
    T &operator*() { return *un_ptr; }
    T *operator->() { return un_ptr; }
};

#endif // !UNIQUE_PTR