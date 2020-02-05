#include <vector>
#include <iostream>
#include <list>
#include <iterator>

using namespace std;

template <typename ForwardIter>
ForwardIter partition(ForwardIter first, ForwardIter last)
{
    ForwardIter j = first;
    auto iter = first;
    advance(iter, 1);

    for (; iter != last; ++iter)
    {
        if (*iter < *first)
        {
            ++j;
            if (j != iter)
            {
                iter_swap(j, iter);
            }
        }
    }
    iter_swap(j, first);
    return j;
}

template <typename ForwardIter>
void quick_sort(ForwardIter first, ForwardIter last)
{
    if (first != last)
    {
        auto mid = partition(first, last);
        quick_sort(first, mid);
        quick_sort(++mid, last);
    }
}
template <typename ForwardIter>
void merge(ForwardIter first, ForwardIter mid, ForwardIter last, ForwardIter dst)
{
    auto mid_begin = mid;
    auto begin = first;
    auto tmp = dst;

    while (begin != mid && mid_begin != last)
    {
        if (*begin <= *mid_begin)
            *(tmp++) = *(begin++);
        else
            *(tmp++) = *(mid_begin++);
    }

    while (begin != mid)
        *(tmp++) = *(begin++);
    while (mid_begin != last)
        *(tmp++) = *(mid_begin++);

    while (first != last)
        *(first++) = *(dst++);
}

template <typename ForwardIter>
void merge_sort(ForwardIter first, ForwardIter last, ForwardIter dst)
{

    int d = distance(first, last);
    if (d > 1)
    {
        auto mid = first;
        auto tmp = dst;
        advance(mid, d / 2);
        merge_sort(first, mid, tmp);

        copy(first, mid, ostream_iterator<typename iterator_traits<ForwardIter>::value_type>(cout, " "));
        cout<<endl;

        advance(tmp, d / 2);
        merge_sort(mid, last, tmp);
        
        copy(mid, last, ostream_iterator<typename iterator_traits<ForwardIter>::value_type>(cout, " "));
        cout<<endl;

        merge(first, mid, last, dst);
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

    list<int> nums = {5, 3, 2, 4, 1, 6, 78, 9};
    list<int> res(8);
    merge_sort(nums.begin(), nums.end(), res.begin());

    for (int i : res)
    {
        cout << i << ' ';
    }
    cout << endl;

    return 0;
}