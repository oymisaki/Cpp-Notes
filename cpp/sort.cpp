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

    int k = first;
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
    for(int i = first; i < last; ++i)
        nums[i] = res[i];
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