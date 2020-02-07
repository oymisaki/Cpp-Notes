#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <numeric>
#include <chrono>
#include <thread>
#include <functional>

using namespace std;

template<typename ForwardIter, typename T>
struct accumulate_temp
{
    void operator()(ForwardIter first, ForwardIter last, T& result)
    {
        result =  std::accumulate(first, last, 0);
    }
};


template<typename ForwardIter, typename T>
T parallel_accumulate(ForwardIter first, ForwardIter last, T init)
{
    int numOfThreads = 8;
    auto accum = accumulate_temp<ForwardIter, T>();


    int offset = distance(first, last) / (numOfThreads - 1);
    // cout << offset << endl;

    auto begin = first;
    auto end = first;

    vector<T> res(numOfThreads);
    vector<thread> threads;
    for(int i = 0; i < numOfThreads - 1; ++i)
    {
        advance(end, offset);
        threads.push_back(std::thread(accum, begin, end, std::ref(res[i])));
        begin = end;
    }
    threads.push_back(std::thread(accum, begin, last, std::ref(res[numOfThreads-1])));

    for(auto& t: threads)
    {   
        t.join();
        // cout << res.back() << endl;
    }

    return accumulate(res.begin(), res.end(), init);
}

// int main()
// {
//     vector<int> nums(10000000, 1);
    
//     auto start = std::chrono::high_resolution_clock::now();
//     cout << parallel_accumulate(nums.begin(), nums.end(), 0) << endl;
//     auto stop = std::chrono::high_resolution_clock::now();
//     cout << chrono::duration<double>(stop - start).count() << endl;

//     start = std::chrono::high_resolution_clock::now();
//     cout << accumulate(nums.begin(), nums.end(), 0) << endl;
//     stop = std::chrono::high_resolution_clock::now();
//     cout << chrono::duration<double>(stop - start).count() << endl;

//     return 0;
// }