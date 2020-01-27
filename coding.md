# coding

## sorting
```cpp
#include <vector>
#include <iostream>

using namespace std;

int partition(vector<int> &nums, int begin, int end)
{
    int j = begin;

    int key = nums[begin];

    for (int i = begin + 1; i < end; ++i)
    {
        if (nums[i] < key)
        {
            ++j;
            if (j != i)
            {
                swap(nums[i], nums[j]);
            }
        }
    }
    swap(nums[j], nums[begin]);
    return j;
}

void quick_sort(vector<int> &nums, int begin, int end)
{
    if (begin < end)
    {
        int mid = partition(nums, begin, end);
        quick_sort(nums, begin, mid);
        quick_sort(nums, mid + 1, end);
    }
}

void merge(vector<int> &nums, int first, int mid, int last, vector<int> &res)
{
    int i = first, j = mid;
    int m = mid, n = last;

    int k = 0;
    while (i < m && j < n)
    {
        if (nums[i] < nums[j])
            res[k++] = nums[i++];
        else
            res[k++] = nums[j++];
    }
    while (i < m)
        res[k++] = nums[i++];
    while (j < n)
        res[k++] = nums[j++];
    for (i = 0; i < k; ++i)
    {
        nums[first + i] = res[i];
    }
}

void merge_sort(vector<int> &nums, int begin, int end, vector<int> &res)
{
    if (begin < end - 1)
    {
        int mid = (begin + end) / 2;
        merge_sort(nums, begin, mid, res);
        merge_sort(nums, mid, end, res);
        merge(nums, begin, mid, end, res);
    }
}

void bubble_sort(vector<int> &nums, int begin, int end)
{
    for (int i = begin; i < end; ++i)
    {
        for (int j = begin; j < end - i - 1; ++j)
        {
            if (nums[j] > nums[j + 1])
            {
                swap(nums[j], nums[j + 1]);
            }
        }
    }
}

void selection_sort(vector<int> &nums, int begin, int end)
{
    for (int i = begin; i < end; ++i)
    {
        for (int j = i + 1; j < end; ++j)
        {
            if (nums[j] < nums[i])
                swap(nums[j], nums[i]);
        }
    }
}

int main()
{

    vector<int> nums = {5, 3, 2, 4, 1};
    vector<int> res(5);
    merge_sort(nums, 0, 5, res);

    for (int i : res)
    {
        cout << i << ' ';
    }
    cout << endl;

    return 0;
}
```

## string

```cpp

#include <iostream>
using namespace std;

int StringToInt(const char *str);
int StringToIntCore(const char *str, bool minus); //字符串转整型核心函数

int StringToInt(const char *str)
{

    int num;
    if (str != NULL && *str != '\0') //str为空指针或空字符串返回零
    {
        //处理正负号
        bool minus = false;
        if (*str == '+')
            str++;
        else if (*str == '-')
        {
            str++;
            minus = true;
        }
        if (*str != '\0') //处理输入只有正负号情况
        {
            num = StringToIntCore(str, minus);
        }
    }
    else
        throw std::invalid_argument("Invalid String Input");

    return num;
}

int StringToIntCore(const char *str, bool minus)
{
    long long num = 0;
    while (*str != '\0')
    {
        if (*str >= '0' && *str <= '9') //转换‘0’~'9'之间的字符
        {
            int flag = minus ? -1 : 1; //条件运算符
            num = num * 10 + flag * (*str - '0');
            //判断num是否溢出
            if ((!minus && num > 0x7FFFFFFF) || (minus && num < (signed int)0x80000000)) 
            //(signed int)0x80000000为int 型的最小负数，必须加signed int
                throw std::overflow_error("Integer Overflow");

            str++; //向后移动一位
        }
        else
        {
            throw std::invalid_argument("Invalid String Input");
        }
    }

    return (int)num;
}

int main()
{
    cout<<StringToInt("111111111111111111")<<endl;
    return 0;
}

```

```cpp

#include <iostream>
#include <assert.h>                          //调用assert函数
using namespace std;                         //使用标准命名空间


char * str_cpy(char *dst, const char *src) //dst长度必须大于等于src，否则会出错
{
    assert(dst != NULL && src != NULL); //保持程序鲁棒性
    char *result = dst;
    while (*src != '\0')
    {
        *(dst++) = *(src++);
    }

    *dst = '\0'; //字符串末尾为'\0'字符
    
    return result;
}
```

## tree

```cpp
#include <vector>
#include <stack>

using namespace std;

class TreeNode
{
public:
    int val;
    TreeNode * left = nullptr;
    TreeNode * right = nullptr;
};

vector<int> postorderTraversal(TreeNode* root)
{
    vector<int> nodes;
    stack<TreeNode *> todo;
    TreeNode* last = nullptr;

    while(root || !todo.empty())
    {
        if(root)
        {
            todo.push(root);
            root = root->left;
        }
        else
        {
            TreeNode * node = todo.top();
            if(node -> right && last != node -> right)
            {
                root = node -> right;
            }
            else
            {
                nodes.push_back(node -> val);
                last = node;
                todo.pop();
            }
        }
    }
}

vector<int> preorderTraversal(TreeNode* root)
{
    vector<int> nodes;
    stack<TreeNode *> todo;
    TreeNode* last = nullptr;

    while(root || !todo.empty())
    {
        if(root)
        {
            nodes.push_back(root->val);
            if(root -> right)
            {
                todo.push(root->right);
            }
            root = root -> left;
        }
        else
        {
            root = todo.top();
            todo.pop();
        }
    }
}

vector<int> inorderTraversal(TreeNode* root)
{
    vector<int> nodes;
    stack<TreeNode *> todo;
    TreeNode* last = nullptr;

    while(root || !todo.empty())
    {
        while(root)
        {
            todo.push(root);
            root = root -> left;
        }

        root = todo.top();
        todo.pop();

        nodes.push_back(root->val);
        root = root -> right;
    }
}

```

## unique_ptr

```cpp
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
        del(un_ptr);
    }

    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;

    // 右值引用 c++ 11
    // 重载右值引用拷贝构造函数
    unique_ptr(unique_ptr &&right_value) : un_ptr(right_value.un_ptr), del(std::move(right_value.del))
    {
        right_value.un_ptr = nullptr;
    }

    // 赋值
    unique_ptr &operator=(unique_ptr &&right_value) noexcept
    {
        if (this != &right_value)
        {
            std::cout << "operator && right_value " << std::endl;
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
    void reset() { del(un_ptr); }
    void reset(T *q)
    {
        if (un_ptr)
        {
            del(un_ptr);
            un_ptr = q;
        }
        else
            un_ptr = nullptr;
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
```

## Vector
```cpp
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

```