namespace ThorsAnvil
{
template <typename T>
class SP
{
    T *data;
    int *count;

public:
    // Explicit constructor
    explicit SP(T *data) try
        : data(data),
          count(new int(1))
    {
    }
    catch (...)
    {
        // If we failed because of an exception
        // delete the pointer and rethrow the exception.
        delete data;
        throw;
    }
    ~SP()
    {
        --(*count);
        if (*count == 0)
        {
            delete data;
            delete count;
        }
    }
    SP(SP const &copy)
        : data(copy.data), count(copy.count)
    {
        ++(*count);
    }
    // Use the copy and swap idiom
    // It works perfectly for this situation.
    SP &operator=(SP rhs)
    {
        rhs.swap(*this);
        return *this;
    }
    SP &operator=(T *newData)
    {
        SP tmp(newData);
        tmp.swap(*this);
        return *this;
    }

    template <typename U>
    friend class shared_ptr;

    template <typename U>
    shared_ptr(const shared_ptr<U> &copying)
        : data(copying.data), counter(copying.counter)
    {
        ++(*counter);
    }

    template <typename U>
    shared_ptr<T> &shared_ptr(const shared_ptr<U> &copying)
    {
        data = copying.data;
        counter = copying.counter;
        ++(*counter);
        return *this;
    }

    // Always good to have a swap function
    // Make sure it is noexcept
    void swap(SP &other) noexcept
    {
        std::swap(data, other.data);
        std::swap(count, other.count);
    }
    // Const correct access owned object
    T *operator->() const { return data; }
    T &operator*() const { return *data; }

    // Access to smart pointer state
    T *get() const { return data; }
    explicit operator bool() const { return data; }
};
} // namespace ThorsAnvil