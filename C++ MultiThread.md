# Multithread

## Basics

+ Re-entrancy
+ Mutual exclusion
  + race conditions
  + deadlock
  + livelock
  + starvation
+ Thread-local storage
+ Atomic operations

### What is thread safety
+ 多线程能够共享同一块数据的同时保证每次只有一个线程在访问共享数据

### 线程与进程的关系
a. 进程是资源分配的基本单位，线程是cpu调度，或者说是程序执行的最小单位。
b. 进程有独立的地址空间，比如在linux下面启动一个新的进程，系统必须分配给它独立的地址空间，建立众多的数据表来维护它的代码段、堆栈段和数据段，这是一种非常昂贵的多任务工作方式。而运行一个进程中的线程，它们之间共享大部分数据，使用相同的地址空间，因此启动一个线程，切换一个线程远比进程操作要快，花费也要小得多。
c. 线程之间的通信比较方便。统一进程下的线程共享数据（比如全局变量，静态变量），通过这些数据来通信不仅快捷而且方便，当然如何处理好这些访问的同步与互斥正是编写多线程程序的难点。
d. 每个独立的线程有有自己的一个程序入口，顺序执行序列和程序的出口，但是线程不能独立执行，必须依附与程序之中，由应用程序提供多个线程的并发控制。

## thread类

### 线程管理
+ `thread(do_some_func)` 启动线程，或是传入调用某个`callable` 类对象，即是一个重载了`operator()`的类。
  当使用函数对象时，避免语法混淆，应该使用
  ```cpp
  std::thread my_thread((background_task())); // 1
  std::thread my_thread{background_task()}; // 2
  ```
+ `my_thread.join()` 等待加入的进程完成后再继续，
+ `my_thread.detach()` 将目前进程分离到后台运行
+ `std::thread t(f, 3, "hello");` 传入参数
  + 如果传入函数的参数中有引用类型，如`f(int&, const int&)`为了正确传参,应该使用
    ```cpp
    std::thread t(f, std::ref(a), std::cref(b));
    ```
  + 对于传入右值，或者不支持拷贝函数的对象，如 `unique_ptr`，使用 `std::move`
    ```cpp
    void process_big_object(std::unique_ptr<big_object>);

    std::unique_ptr<big_object> p(new big_object);
    p->prepare_data(42);
    std::thread t(process_big_object,std::move(p));
    ```
    其中，由于 `p` 是 `unique_ptr` 不支持拷贝构造，因此要调用它的移动构造函数
+ `thread` 可移动不可复制，将thread对象作为参数的两种方式. `std::move` or `&`.前者会让出管理权。利用 `move-aware containers` 量产线程. 注意thread一旦创建就开始运行了, `join()`仅仅意味着等待.
  ```cpp
  void do_work(unsigned id);

  void f()
  {
    std::vector<std::thread> threads;
    for(unsigned i=0; i < 20; ++i)
    {
      threads.push_back(std::thread(do_work,i)); // 产生线程
    } 
    std::for_each(threads.begin(),threads.end(),
                    std::mem_fn(&std::thread::join)); // 对每个线程调用join()
  }
  ```
  错误多线程示范. 下面这个程序实际上是逐个线程运行的.
  ```cpp
  int i;
  for (i = 0; i < 10; i++) {
      thread t1(nThre);
      t1.join();
      cout << "Joined thread n'" << i << '\n';
  }
  ```
+ 标识线程. `get_id()` ，可以结合 `std::this_thread` 来辨别主线程.
+ 条件竞争. **Eg.** 对于有三个节点的链表，访问一个节点，必须防止对三个节点的并发访问.


## mutex类

### 须知
+ 锁的粒度大，则竞争多，粒度小，则有可能会不起作用。如 `线程安全栈`， 为什么标准库里的栈线程不安全，因为`top()`返回引用，会将内部数据暴露在外面。同时调用 `empty()` 之后也可能会有其他进程 `pop()`。

**使用互斥量保护共享数据**
### lock_guard
Always use `std::lock_guard` rather than calling `lock()` and `unlock()` on the `mutex`. The latter is far more error prone and not exception safe.

在 lock_guard 对象构造时，传入的 Mutex 对象(即它所管理的 Mutex 对象)会被当前线程锁住。在lock_guard 对象被析构时，它所管理的 Mutex 对象会自动解锁，由于不需要程序员手动调用 lock 和 unlock 对 Mutex 进行上锁和解锁操作，因此这也是最简单安全的上锁和解锁方式，**尤其是在程序抛出异常后先前已被上锁的 Mutex 对象可以正确进行解锁操作，极大地简化了程序员编写与 Mutex 相关的异常处理代码**。

指针具有访问能力, 因此要尽可能锁住任何对保护数据的访问。

**一般写一个类，把共享数据封装起来**

**用std::lock避免死锁**
要同时拥有鼓和锤才能打鼓，一个人拿了鼓，一个人拿了锤。

+ 解决办法一。总是以相同的上锁，A锁总是在B锁之前申请。
+ 解决办法二。`std::lock` lock the mutexes all at a time.

**这是一个非常典型的死锁的例子**
```cpp
// 这里的std::lock()需要包含<mutex>头文件
class some_big_object;
void swap(some_big_object& lhs,some_big_object& rhs);
class X
{
private:
  some_big_object some_detail;
  std::mutex m;
public:
  X(some_big_object const& sd):some_detail(sd){}

  friend void swap(X& lhs, X& rhs)
  {
    if(&lhs==&rhs)
      return;
    std::lock(lhs.m,rhs.m); // 1
    std::lock_guard<std::mutex> lock_a(lhs.m,std::adopt_lock); // 2
    std::lock_guard<std::mutex> lock_b(rhs.m,std::adopt_lock); // 3
    swap(lhs.some_detail,rhs.some_detail);
  }
};
```

**避免死锁的进阶指导**
+ 尽可能只获得一个锁
+ 规定顺序
+ hierarchical_mutex

### unique_lock
可以更加灵活. 作用和`lock_guard` 类似，但是可以手动进行 `unlock()` 操作，从而可以灵活控制锁的**粒度**.

```cpp
void get_and_process_data()
{
  std::unique_lock<std::mutex> my_lock(the_mutex);
  some_class data_to_process=get_next_data_chunk();
  my_lock.unlock();  // 1 不要让锁住的互斥量越过process()函数的调用
  result_type result=process(data_to_process);
  my_lock.lock(); // 2 为了写入数据，对互斥量再次上锁
  write_result(data_to_process,result);
}
```


### Thread类

+ 默认构造函数，创建一个空的 thread 执行对象。
+ 初始化构造函数，创建一个 thread对象，该 thread对象可被 joinable，新产生的线程会调用 fn 函数，该函数的参数由 args 给出。
+ 拷贝构造函数(被禁用)，意味着 thread 不可被拷贝构造。
+ move 构造函数，move 构造函数，调用成功之后 x 不代表任何 thread 执行对象。
```cpp
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
 
void f1(int n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread " << n << " executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
 
void f2(int& n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 2 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
 
int main()
{
    int n = 0;
    std::thread t1; // t1 is not a thread
    std::thread t2(f1, n + 1); // pass by value
    std::thread t3(f2, std::ref(n)); // pass by reference
    std::thread t4(std::move(t3)); // t4 is now running f2(). t3 is no longer a thread
    t2.join();
    t4.join();
    std::cout << "Final value of n is " << n << '\n';
}
```

## join
`thread.join()` 会让当前线程等待 加入的`tread`执行完再往后执行。

## std::this_thread::yield
yield 让出时间片和CPU让其他程序执行. `while(!Done) yield()`. 当前线程并没有停止，而是让出了优先级，执行完新的任务后会回过头询问。

## mutex 
既不允许复制构造也不允许移动拷贝。

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::mutex m;//you can use std::lock_guard if you want to be exception safe
int i = 0;

void makeACallFromPhoneBooth() 
{
    m.lock();//man gets a hold of the phone booth door and locks it. The other men wait outside
      //man happily talks to his wife from now....
      std::cout << i << " Hello Wife" << std::endl;
      i++;//no other thread can access variable i until m.unlock() is called
      //...until now, with no interruption from other men
    m.unlock();//man lets go of the door handle and unlocks the door
}

int main() 
{
    //This is the main crowd of people uninterested in making a phone call

    //man1 leaves the crowd to go to the phone booth
    std::thread man1(makeACallFromPhoneBooth);
    //Although man2 appears to start second, there's a good chance he might
    //reach the phone booth before man1
    std::thread man2(makeACallFromPhoneBooth);
    //And hey, man3 also joined the race to the booth
    std::thread man3(makeACallFromPhoneBooth);

    man1.join();//man1 finished his phone call and joins the crowd
    man2.join();//man2 finished his phone call and joins the crowd
    man3.join();//man3 finished his phone call and joins the crowd
    return 0;
}
```

## lock_guard
Always use `std::lock_guard` rather than calling `lock()` and `unlock()` on the `mutex`. The latter is far more error prone and not exception safe.

在 lock_guard 对象构造时，传入的 Mutex 对象(即它所管理的 Mutex 对象)会被当前线程锁住。在lock_guard 对象被析构时，它所管理的 Mutex 对象会自动解锁，由于不需要程序员手动调用 lock 和 unlock 对 Mutex 进行上锁和解锁操作，因此这也是最简单安全的上锁和解锁方式，**尤其是在程序抛出异常后先前已被上锁的 Mutex 对象可以正确进行解锁操作，极大地简化了程序员编写与 Mutex 相关的异常处理代码**。

## thread safety

## yield vs. condition_variable

`while(!condition) yield();` will constantly ask the cpu for some resources.
`while(!not_ready) cv.wait()` will make the thread skeep until `cv.notify()` wakes it up.
