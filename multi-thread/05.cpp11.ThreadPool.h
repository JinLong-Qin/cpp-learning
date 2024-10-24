#pragma once
/*
    构成：
        1.管理者线程 子线程 1个
            - 控制工作线程的数量，增加或者减少
        2.工作线程 子线程 n个
            - 从任务队列中取任务并执行
            - 任务队列为空，就被阻塞，使用条件变量
            - 线程同步，使用互斥锁
            - 当前数量，空闲的线程数量
            - 指定最大最小线程数量
        3.任务队列 stl.queue
            - 互斥锁
            - 条件变量
        4.线程池开关 bool
*/

/*
原子指的是一系列不可被CPU上下文交换的机器指令，这些指令组合在一起就形成了原子操作。在多核CPU下，当某个CPU核心开始运行原子操作时，会先暂停其它CPU内核对内存的操作，以保证原子操作不会被其它CPU内核所干扰。
由于原子操作是通过指令提供的支持，因此它的性能相比锁和消息传递会好很多。相比较于锁而言，原子类型不需要开发者处理加锁和释放锁的问题，同时支持修改，读取等操作，还具备较高的并发性能，几乎所有的语言都支持原子类型。
*/
#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <map>
#include <future>
#include <memory>

using namespace std;

class ThreadPool
{
private:
    thread * m_manager; // 管理者线程
    map<thread::id, thread> m_workers;
    vector<thread::id> m_ids; // 存储已经推出了任务函数的id
    // 定义为原子变量，当不能保证一个变量在多线程中不会被修改的时候，就可以将其定义为原子变量
    atomic<int> m_minThread; // 最小线程数量
    atomic<int> m_maxThread; // 最大线程数量
    atomic<int> m_curThread; // 当前线程数量
    atomic<int> m_idleThread; // 空闲线程数量
    atomic<int> m_exitThread; // 要退出的线程数量
    atomic<bool> m_stop; // 线程池开关
    // ！！！！原子变量的++ --操作是线程安全的，其他时候尽量使用load()函数读数据，store()函数写数据

    // 任务队列 
    // 存储的就是函数地址，直到被线程取出并传递对应的参数才会开始运行，
    // 所以这里面存储的是函数指针
    queue<function<void(void)>> m_tasks;
    /*可调用对象包装器
    
    它是一个类模板，可以容纳除了类(非静态)成员（函数）指针之外的所有可调用对象。通过指定它的模板参数，它可以用统一的方式处理函数、函数对象、函数指针，并允许保存和延迟执行它们。
    function<返回值类型(参数类型列表)> diy_name = 可调用对象;

    function可以将可调用对象进行包装，得到一个统一的格式，包装完成得到的对象相当于一个函数指针，和函数指针的使用方式相同，通过包装器对象就可以完成对包装的函数的调用了。


    */


    // 锁
    mutex m_queueMutex; // 队列的锁
    mutex m_idsMutex; // ids的锁
    // 条件变量 用来阻塞消费者线程，当任务队列为空，就阻塞消费者线程，当生产者增加任务，就放开消费者线程
    condition_variable m_condition;



public:
    ThreadPool(int min=2, int max=thread::hardware_concurrency()); // 最大线程数量一般都和cpu的核心数相同, 使用thread::hardware_concurrency来找到cpu的核心数
    ~ThreadPool();

    // 添加任务到任务队列中
    // 任务对象保存的是可调用对象，所以增加task的时候，增加的应该也要是可调用对象
    void addTask(function<void(void)> task);

    // 为了要让添加任务的函数更加灵活和为了实现线程异步，考虑修改为模板函数
    // typename... Args 代表0个或多个模板参数，使用Args...来展开参数列表
    template<typename F, typename... Args>
    // 前面返回值写auto，真正返回值类型写在->后面，想要获取函数类型需要用[类型萃取器: result_of<> 也是一个模板类型]
    // result_of<F(Args...)>::type  这表示，F函数在通过使用Args...这些参数之后，执行出来的返回值的 类型（::type）
    // 但是由于这是个泛型，编译器不明确这一个东西具体是什么，所以增加typename关键字告诉编译器这是一个数据类型

    // 另：
    /*假设有一 template<typename T> 
        对于泛型编程而言，T&& 被称为未定引用类型。他具体的引用类型要根据传递进来的实参具体决定，
            如果实参是一个字面量（右值，无法取地址），如10，那么T&&就是右值引用类型。10传递进来就是 T&&a -> int&&
                右值引用  T&&绑定到右值（临时对象或字面量，不能取地址）。主要用于实现移动语义，将资源从一个对象“移动”到另一个对象，而不是复制。
                    int&& rref = 10; // rref 是 10 的右值引用
            如果实参传递进来的实参是一个变量（左值，可以取地址），如a，那么T&&就是一个左值引用类型。int a; 传递进来 T&& -> int& 
                左值引用  T&绑定在左值（具有持久状态的对象，可以取地址），通常是用于修改对象或延长对象的生命周期
                    int a = 10;
                    int& ref = a; // ref 是 a 的左值引用
                    ref = 20; // 修改 a 的值
    */
    auto addTask(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type> { // 模板函数的定义和实现要写在一起，通常文件名为.hpp后缀
        // 1. package_task<函数返回值(函数参数)>
        using returnType = typename result_of<F(Args...)>::type;
        // packaged_task<returnType()>> pkg(
        // 使用智能指针为创建的可调用对象包装器类对象分配一片内存，否则pkg在加入到mtasks的时候，会复制一份函数放置到mtasks中，并且当addtask函数执行完之后，栈中原本的pkg函数会销毁，其复制的函数调用也会出现问题，因此要将其创建在堆上，使用智能指针实现这个功能
        auto myTask = make_shared<packaged_task<returnType()>>(
            /*
            // 这里参数为变参的，所以使用可调用对象绑定器进行绑定，如果有参数，就已经绑定了，没有参数那也没关系
            // 但是由于是泛型编程，所以不确定实际类型。使用forward[完美转发]进行参数绑定
            // forward函数会通过F的具体类型，来保证对f做实际的类型传递
                // 原理是，先通过实参来确定F的实际类型，判断其是左值引用还是右值引用，
                    // 如果是左值引用，f就会通过forward函数做左值引用的传递，否则就按照右值传递
                    // ！！forward函数会对其参数f，根据模板类型F进行处理，转发的就是其返回值，要么左值要么右值
            // !!通过完美转发，可以保证数据的原始类型，否则可能在传递过程中左值变右值。
            */
            bind(forward<F>(f), forward<Args>(args)...)
 
        );
        // 2. 得到future，其中保存着函数的返回值
        future<returnType> res = myTask->get_future();
        // 3. 任务函数添加到任务队列中
        m_queueMutex.lock(); // 可以这样加锁，也可以使用模板类 lock_guard / unique_lock; 模板类加锁的好处是，可以不用手动解锁，在执行完代码块的时候就会自动解锁
        // mtasks中保存的而是函数，所以使用lambda表达式包装一个函数然后保存进去，
        // 这里如果给定的是pkg，那么就会复制一个pkg进去，但是pkg创建在栈上，函数执行完之后会随着栈销毁，那么其复制进去的pkg做执行的时候也会出问题
        // 所以使用智能指针在将pkg保存在堆上
        m_tasks.emplace([myTask](){
            (*myTask)();  // myTask是指针，所以先解引用，再执行
        });
        m_queueMutex.unlock(); // 手动解锁
        m_condition.notify_one(); // 添加好了任务，唤醒一个阻塞线程
        return res;
    }

private:
    void manager(void);
    void worker(void);
};

