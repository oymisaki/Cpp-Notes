#include <cstddef>
#include <stdexcept>
#include <memory>
#include <iterator>

template <typename T>
class vector
{
public:
    using value_type = T;
    using iterator = value_type *;
    using const_iterator = const value_type *;
    using size_type = std::size_t;

public:
    vector() = default;
    ~vector();

    iterator begin() const;
    iterator end() const;
    size_type size() const;

    value_type &operator[](size_type i) const;
    value_type &at(size_type i) const;

    void push_back(const value_type &new_elem);
    void pop_back();

public:
    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);

private:
    iterator startptr = nullptr;
    iterator endptr = nullptr;
    iterator capptr = nullptr;
    std::allocator<value_type> alloc;

private:
    void check_cap();
    void free();
};

template <typename T>
typename vector<T>::iterator vector<T>::begin() const
{
    return startptr;
}

template <typename T>
typename vector<T>::iterator vector<T>::end() const
{
    return endptr;
}

template <typename T>
typename vector<T>::size_type vector<T>::size() const
{
    return endptr - startptr;
}

template <typename T>
typename vector<T>::value_type &vector<T>::operator[](size_type i) const
{
    return *(startptr + i);
}

template <typename T>
typename vector<T>::value_type &vector<T>::at(size_type i) const
{
    if (startptr + i >= endptr)
    {
        throw std::runtime_error("out of range!");
    }
    return *(startptr + i);
}

template <typename T>
vector<T>::~vector()
{
    free();
}

// allocator 类
// alloc.destroy() 析构对象
// alloc.deallocate() 解分配对象
// alloc.allocate() 分配
template <typename T>
void vector<T>::free()
{
    if (startptr)
    {
        for (auto p = startptr; p != endptr; p++)
        {
            alloc.destroy(p);
        }
        alloc.deallocate(startptr, endptr - startptr);
    }
}

// uninitialized_copy 把内容搬到一个未初始化的空间里去
// make_move_iterator, 从参数类型推断出move_iterator类模板的模板参数

template <typename T>
void vector<T>::check_cap()
{
    if (endptr == capptr)
    {
        int newsize = size() ? size() << 1 : 1;
        auto newstartptr = alloc.allocate(newsize);
        auto newendptr = uninitialized_copy(std::make_move_iterator(startptr), std::make_move_iterator(endptr), newstartptr);
        free();
        startptr = newstartptr;
        endptr = newendptr;
        capptr = newstartptr + newsize;
    }
}

template <typename T>
void vector<T>::push_back(const value_type &new_elem)
{
    check_cap();
    alloc.construct(endptr, new_elem);
    endptr++;
}

template <typename T>
void vector<T>::pop_back()
{
    if (endptr - startptr > 0)
    {
        alloc.destroy(endptr);
        endptr--;
    }
}

// 高级函数
template <typename T>
typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last)
{
    if (last >= endptr || first < startptr)
        throw std::runtime_error("out of range!");
    iterator newendptr = std::copy(last, static_cast<const_iterator>(endptr), first);
    while (newendptr < endptr)
    {
        alloc.destroy(--endptr);
    }
    return endptr;
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(const_iterator position)
{
    return erase(position, position + 1);
}
