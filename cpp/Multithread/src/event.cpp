#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

class event
{
private:
    vector<pair<int, function<void()>>> events;
    bool started = false;
    int timer;

public:
    event() {}

    void add_event(int t, function<void()> e)
    {
        auto f = [=] {
            std::this_thread::sleep_for(chrono::duration<int>(t));
            cout << t << endl;
            e();
        };

        events.emplace_back(t, f);
    }

    void set_timer(int t)
    {
        timer = t;
    }

    void start_event()
    {
        typedef pair<int, function<void()>> pif;
        auto last = partition(events.begin(), events.end(),
                              [this](const pif &elem) { return elem.first >= timer; });
        vector<thread> threads;
        for(auto iter = events.begin(); iter != last; ++iter)
        {
            cout << iter->first << endl;
            threads.emplace_back(iter->second);
        }


        // 一定要join
        for (auto &t : threads)
        {
            t.join();
        }
    }
};

int main()
{
    event x;

    x.add_event(3, []() { cout << "first" << endl; });
    x.add_event(4, []() { cout << "second" << endl; });
    x.add_event(5, []() { cout << "third" << endl; });

    x.set_timer(4);

    x.start_event();

    return 0;
}
