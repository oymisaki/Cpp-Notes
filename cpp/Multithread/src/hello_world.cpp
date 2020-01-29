#include <iostream>
#include <thread>

using namespace std;

void hello()
{
    cout<<"Hello Concurrent world" <<endl;
}

int main(int argc, const char** argv) {

    thread t(hello); // 启动线程
    t.join();

    return 0;
}