#include "05.cpp11.ThreadPool.h"
#include <iostream>



// 写在大括号中是赋值，写在初始化列表中叫做初始化；初始化的效率要比赋值高
// 这里默认为，最开始创建的最少数量的线程数，就是刚开始空闲线程数和当前线程数的初值
ThreadPool::ThreadPool(int min, int max): m_maxThread(max), m_minThread(min), m_stop(false), m_curThread(max/2), m_idleThread(max/2)
{
    cout << "max = " << max << endl;
    // 创建管理者线程
    m_manager = new thread(&ThreadPool::manager, this); // 线程的处理函数，线程的所有者； 如果线程的处理函数，不是类的成员函数，可以不指定线程所有者
    // 工作线程
    for (int i = 0; i < max/2; ++i)
    {
        thread t(&ThreadPool::worker, this);
        m_workers.insert(make_pair(t.get_id(), move(t)));  // 使用move操作将临时变量转移到map中，否则局部变量在代码段执行完后会被销毁
    }
    
}

void ThreadPool::addTask(function<void(void)> task)
{
    {
        // 管理互斥锁的模板类lock_guard，unique_lock；这两个模板类作用和使用方法是一样的
        lock_guard<mutex> locker(m_queueMutex); // 会在locker对象创建的时候，同时对m_queueMutex进行加锁，// 当代码段（一对对应的大括号）执行结束，locker对象会被析构，在析构中会将m_queueMutex解锁
        // unique_lock
        // m_tasks.push(task);
        // queue提供了emplace方法增加了添加队列信息的效率，但是只有在调用方法的时候构建匿名对象才有用。这里给定了task参数，所以调用emplace和push，效率是一样的
        m_tasks.emplace(task);      
    } // 使用大括号，改变locker的作用域，则执行到这里，locker会先被析构掉，同时对m_queueMutex解锁
    

    // 当添加了任务之后，就唤醒被阻塞的工作线程；； 当任务队列为空的时候，工作线程会被阻塞
    m_condition.notify_one(); // 使用条件变量，唤醒一个线程
    // m_condition.notify_all(); // 使用条件变量，唤醒所有被阻塞的线程

    
}

void ThreadPool::manager(void)
{
    while (!m_stop.load()) 
    { // 当线程池没有关闭
        // 线程休眠3秒钟，即每3秒执行一次后面代码
        this_thread::sleep_for(chrono::seconds(1));

        int idel = m_idleThread.load(); // 休眠的线程数
        int cur = m_curThread.load(); // 当前所有的线程数
        
        // 线程退出
        // !!自定义条件。当空闲线程大于总线程数一半 并且，当前线程数大于最小线程数的时候,要根据实际业务情况来设定条件
        // 空闲线程并不是没有事做，而是被阻塞了
        if (idel>cur/2 && cur>m_minThread)
        { 
            // 每次销毁两个线程
            m_exitThread.store(2); // 原子操作，m_exitThread保存2的值 等价于m_exitThread=2，但是这样线程安全
            // 唤醒所有被阻塞的线程，让工作线程自己去销毁自己
            m_condition.notify_all();

            lock_guard<mutex> lck(m_idsMutex); // 为ids加锁，在运行完一个代码块(对应的大括号)就会自动析构掉lck，同时解锁
            for(auto id: m_ids){ // 遍历mids ，这里面保存的是线程id
                auto it = m_workers.find(id); // 在mworkers中找到对应线程id 的对象， 是pair对象，first是id second是线程对象
                if(it != m_workers.end()){
                    cout << "=====管理者线程将工作线程， id " << (*it).first << " 销毁了" << endl;
                    (*it).second.join(); // 回收线程资源
                    m_workers.erase(it); // 销毁线程对象
                    
                }
            }
            m_ids.clear();

        }else if(idel == 0 && cur < m_maxThread){ // 线程的添加, 当空闲为0，并且当前的线程数小于最大线程数  要根据实际业务情况来设定条件
            thread t(&ThreadPool::worker, this);
            m_workers.insert(make_pair(t.get_id(), move(t)));
            ++m_curThread;
            ++m_idleThread;
        }

        
        
    }
    
}

void ThreadPool::worker(void)
{
    // 进入到任务队列中，不断的循环从任务队列中找任务.
    // 1. 判断任务队列是否为空，为空就阻塞
    // 2. 不为空就取任务做执行
    while (!m_stop.load()) // 使用原子变量的load函数，线程安全的取值，
    { // 当线程池没有关闭
        function<void(void)> task = nullptr; // 可调用对象的初始化，
        {
            // 控制锁模板类 unique_lock
            unique_lock<mutex> locker(m_queueMutex); // 加锁

            // if(m_tasks.empty() && m_stop){ // 当addTask函数中的m_condition.notify_one();被执行后，会有线程被唤醒，然后再加锁执行后面的内容；
            while(m_tasks.empty() && !m_stop){ // 但是如果是notify_all的话，就会唤醒所有被阻塞的线程，他们来抢这个互斥锁，谁抢到了，谁就加锁然后往后执行，只用if判断就不够了，因此要改为使用while来不断的做判断
                // 任务队列为空，或者线程池没有关闭了
                m_condition.wait(locker); // 上面使用unique lock的原因是由于这里需要一个uniquelock的参数
                // 条件变量调用wait函数时，locker会自动解锁，防止死锁

                if(m_exitThread.load() > 0){
                    --m_curThread;
                    --m_idleThread;
                    --m_exitThread;
                    cout << "---------工作线程退出任务列表, id: " << this_thread::get_id() << endl;
                    lock_guard<mutex> lck(m_idsMutex);
                    m_ids.emplace_back(this_thread::get_id()); // 保存退出线程的id

                    return;
                }
            }

            if(!m_tasks.empty()){
                // task = m_tasks.front(); // 取出队列中队头来对task赋值，但是这会造成资源的浪费，（赋值操作导致的）
                // c++11提供了move操作
                task = move(m_tasks.front()); // 将队头内容直接移动到task中

                m_tasks.pop(); // 弹出队头

                cout << "工作线程从任务列表中取出了一个任务." << endl;
            }
        } // 使用大括号改变locker的作用域，当执行到这里，locker会自动析构，同时解锁
        if(task){ // 如果task不为空，说明取到了对应的可执行
            --m_idleThread; // 开始执行任务， 则空闲线程数-- ；  m_idleThread是原子类，所以线程安全
            task();
            ++m_idleThread; // 执行完毕，空闲线程++
        }

    }
    
}

ThreadPool::~ThreadPool()
{
    m_stop=true; // 标志线程池停止运行了
    m_condition.notify_all(); // 唤醒所有工作线程
    // 工作线程
    for(auto& it: m_workers){ // 使用引用的方式去遍历，因为线程对象时不允许拷贝的
        thread& t = it.second; // 引用类型的迭代器，不需要解引用,直接点就可以得到
        if(t.joinable()){ // 判断，当前线程对象是否可连接，
        // 当线程调用了join方法，或者detach方法的线程就不可连接了
            // ！！！如果不需要主线程等待子线程结束，可以使用detach方法将线程分离。[分离后的线程会在后台独立运行，不会被自动回收，需要自行管理]
        // 当线程调用了join方法或者detach方法后，就不需要再对这个线程做处理了，
            cout << "****析构函数中，工作线程 " << t.get_id() << " 将要退出了." << endl;
            t.join(); // 阻塞当前线程，等待子线程结束，同时回收子线程资源 栈内存，线程句柄(线程的唯一标识符，用于引用和操作线程)
        } // 这里只是退出了任务函数，线程对象并没有被销毁，因为线程对象没有从容器中弹出
    } // 工作线程都是保存在栈内的，所以在程序执行结束后，会随着栈一同销毁

    // 管理者线程
    if(m_manager->joinable()){
        m_manager->join();
    }
    // 因为是new出来的，所以要delete掉
    delete m_manager;

}

