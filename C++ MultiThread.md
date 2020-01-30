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

`adopt_lock` 和 `defer_lock` 体现在创建`lock_guard`对象和取得锁的时机不一样.
```cpp
std::unique_lock<std::mutex> lk1(e1.m, std::defer_lock);
std::unique_lock<std::mutex> lk2(e2.m, std::defer_lock);
std::lock(lk1, lk2);

std::lock(lhs.m, rhs.m);
std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
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

### Lazy initialization -- 锁以外的替代措施
单线程的典型延迟初始化例程.
```cpp
std::shared_ptr<some_resource> resource_ptr;
void foo()
{
  if(!resource_ptr)
  {
    resource_ptr.reset(new some_resource);  // 先检查是已经被初始化
  }
  resource_ptr->do_something();
}
```

线程安全的写法如下，由于每次初始化都会加锁，使得效率变低，如果采用**双重检查锁**（在获取锁之前就事先调用资源进行检查，加锁之后再检查）的方式虽然会提高效率，但是会引入**条件竞争**的问题。
```cpp
std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;

void foo()
{
  std::unique_lock<std::mutex> lk(resource_mutex);  // 所有线程在此序列化 
  if(!resource_ptr)
  {
    resource_ptr.reset(new some_resource);  // 只有初始化过程需要保护 
  }
  lk.unlock();
  resource_ptr->do_something();
}
```

最好的写法，使用 `call_once` `once_flag` 而不是 `mutex`锁
```cpp
std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag;  // 1

void init_resource()
{
  resource_ptr.reset(new some_resource);
}

void foo()
{
  std::call_once(resource_flag,init_resource);  // 可以完整的进行一次初始化
  resource_ptr->do_something();
}
```

### Reader-Writer Lock
有时需要共享读，互斥写，可以使用 `boost::shared_lock`，`std::shared_lock` since `c++ 17`.

`boost::shared_lock`允许多个线程同时对某个共享数据上锁。在读者写者模式时，读者上共享锁，写者上独占锁。

```cpp
class database
{
  int data;
  mutable boost::shared_mutex entry_mutex;

public:
  int reader() const
  {
    boost::shared_lock<boost::shared_mutex> lk(entry_mutex);
    return data;
  }

  void writer() const
  {
    std::lock_guard<boost::shared_mutex> lk(entry_mutex);
    // ... do something 
  }

};
```

## 同步并发操作 -- condition variables and futures

让某线程等待另一个进程完成任务
+ 反复询问。占用CPU
+ 固定等待间隙
  ```cpp
  bool flag;
  std::mutex m;

  void wait_for_flag()
  {
  std::unique_lock<std::mutex> lk(m);
  while(!flag)
  {
    lk.unlock();  // 1 解锁互斥量
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 2 休眠100ms
    lk.lock();   // 3 再锁互斥量
  }
  ```
+ 等待条件达成

### condition variable
定义在 `<condition_variable>` 头文件中.
+ `notify_one()` 唤醒一个线程
+ `wait(lock, []{return !data.empty()})` 等待直到谓词，起到了替代外部循环的作用
  ```cpp
  while(data.empty())
  {
    cv.wait(lock);
  }

  // 等价于
  cv.wait(lock, []{return !data.empty()});
  ```
  之所以是要循环确认，是因为不能保证睡眠的线程**被谁唤醒**


**生产者消费者模型**

```cpp
std::mutex mut;
std::queue<int> data_queue;  // 1
std::condition_variable cv;

void producer_thread()
{
  std::lock_guard<std::mutex> lk(mut);
  // produce
  cv.notify();
}

void consumer_thread()
{
  while(true)
  {
    std::unique_lock<std::mutex> lk(mut);
    cv.wait(lk, []{return !data_queue.empty();});
    
    // consume

    lk.unlock();

    // process
  }
}
```

消费者不能使用 `lock_guard` 的原因是
+ `wait()` 可能会反复调用`unlock()`，每次wait都会调用unlock
+ 为了粒度和效率考虑，获得数据后就释放锁

### 等待一次性事件

**头文件<future>**

等待一个事件或者接受一个`result`，与 `std::async` 可以一起用, `std::async(func)`会在执行完后返回一个`future<>()`对象，
需要使用对象时调用，`future_obj.get()` 来获取结果，并且如果此时结果没有计算完成还会 `block` 当前进程.

**`std::async` 几种传入参数的方式**
+ `std::async(&X::foo,&x,42,"hello")`
+ `std::async(&X::bar,x,"goodbye")` x会拷贝
+ `std::async(Y(),3.141)` 移动构造
+ `std::async(std::ref(y),2.718)`  调用y(2.718)

```cpp
#include <future>
#include <iostream>

int find_the_answer_to_ltuae();
void do_other_stuff();
int main()
{
  std::future<int> the_answer=std::async(find_the_answer_to_ltuae);
  do_other_stuff();
  std::cout<<"The answer is "<<the_answer.get()<<std::endl;
}
```

### 传递和封装任务 -- package_task

**生成一个package_task对象并绑定到某future**
```cpp
std::packaged_task<void()> task(f);  
// 
std::future<void> res=task.get_future(); 
```

### std::promises
Lowest level

### shared_future -- 可拷贝的future
因为提供了拷贝机制，因此可以被多个线程拥有，因此可以让多个现场同时等待一个`future

### future and async, package_task, promises

**async, package_task, promises彼此类似并且逐渐深入底层**

+ `async`是最理想化的，在给`async`传入参数的同时创建了线程并且只能接收一个返回值。
+ `package_task` 则把 `async` 做的工作拆开，`package_task`专门将要执行的工作封装成一个`callable`，什么时候创建进程并调用`task`由用户决定，并且`package_task`在创建时就与`future`绑定
+ `promises` 更往底层，可以用来实现 `package_task`

## C++内存模型和原子操作

### Memory Order

**为什么需要Memory Order**
> 即使是简单的语句，C++ 也不保证是原子操作。
> CPU 可能会调整指令的执行顺序。
> 在 CPU cache 的影响下，一个 CPU 执行了某个指令，不会立即被其它 CPU 看见。

为了有助于理解，参见

[C++ atomics and memory ordering](https://bartoszmilewski.com/2008/12/01/c-atomics-and-memory-ordering/)

[What does memory model in C++ mean?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g)

[理解C++中的Memory Order](http://senlinzhan.github.io/2017/12/04/cpp-memory-order/)

### atomic模板类 -- 原子类型

**原子操作没有中间状态，要么已完成，要么未开始**
+ `load()` 取值
+ `store()` 存值
+ `exchange()` 取出旧值，存入新值
+ `fetch_add()` 即是`+=`

所有的操作都支持 `memory_order` 参数


`Relaxed ordering`
仅仅保证单线程内有序，CPU指令不会被打乱
> You and a friend both agree that x=false and y=false. One day, you send him a letter telling him that x=true. The next day you send him a letter telling him that y=true. You definitely send him the letters in the right order.

> Sometime later, your friend receives a letter from you saying that y=true. Now what does your friend know about x? He probably already received the letter that told him x=true. But maybe the postal system temporarily lost it and he'll receive it tomorrow. So for him, x=false and x=true are both valid possibilities when he receives the y=true letter.

> So, back to the silicon world. Memory between threads has no guarantee at all that writes from other threads turn up in any particular order, and so the 'delayed x' is totally a possibility. All adding an atomic and using relaxed does is stop two threads racing on a single variable from becoming undefined behaviour. It makes no guarantees at all to ordering. Thats what the stronger orderings are for.

`memory_order_seq_cst`
> seq_cst stores require flushing the store buffer so this thread's later reads are delayed until after the store is globally visible.

不会受Cache影响，先进行的读写一定会被**观察**到
**只有一个全序** 当一个顺序被观察到时，就不可能以其他顺序被观察到。

其中`memory_order_release` 和 `memory_order_acquire`

不允许CPU打乱多个线程的读写操作
+ 在store()之前的所有写操作，不允许被移动到这个store()的后面。
+ 在load()之后的所有读操作，不允许被移动到这个load()的前面

**真难**


