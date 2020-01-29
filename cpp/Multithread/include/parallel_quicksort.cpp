#include <list>
#include <future>

template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
    if (input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const &pivot = *result.begin();

    auto divide_point = std::partition(input.begin(), input.end(),
                                       [&](T const &t) { return t < pivot; });

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(),
                      divide_point);

    std::future<std::list<T>> new_lower( // 1
        std::async(&parallel_quick_sort<T>, std::move(lower_part)));

    auto new_higher(
        parallel_quick_sort(std::move(input))); // 2

    result.splice(result.end(), new_higher);        // 3
    result.splice(result.begin(), new_lower.get()); // 4
    return result;
}