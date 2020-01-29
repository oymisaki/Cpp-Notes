#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>

class data_chunk
{
};

std::mutex mut;
std::queue<int> data_queue; // 1
std::condition_variable data_cond;

void data_preparation_thread()
{
    while (true)
    {
        int data = 1;
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);  // 2
        data_cond.notify_one(); // 3
    }
}

void data_processing_thread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(mut); // 4
        data_cond.wait(
            lk, [] { return !data_queue.empty(); }); // 5
        int data = data_queue.front();
        data_queue.pop();
        lk.unlock(); // 6
        data += 1; // process
    }
}