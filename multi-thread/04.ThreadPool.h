#pragma once
#include "04.TaskQueue.h"
#include "04.TaskQueue.cpp"

template <typename T>
class ThreadPool<T>{ 
    // c++封装的思想，要将一些数据隐藏起来，隐藏起来的数据就是成员变量，基于这些属性提供一些api函数，又因为类里面的成员函数可以直接操作成员变量，因此函数的参数就少了，原因就是因为有些属性被隐藏了
    // 如果想要某些函数在类定义出来就存在，就给类增加静态标识符，静态函数属于类，不属于类实例出来的对象
public:
    ThreadPool(int min, int max);
    ~ThreadPool();

    // 添加任务
    void addTask(Task<T> task);
    // 获取忙线程的个数
    int getBusyNumber();
    // 获取活着的线程个数
    int getAliveNumber();

private: // 不需要外部调用
    // 工作的线程的任务函数
    static void* worker(void* arg);
    // 管理者线程的任务函数
    static void* manager(void* arg);
    void threadExit();

private:
    // 锁
    pthread_mutex_t m_lock;
    // 条件变量，是否任务列表是空的
    pthread_cond_t m_notEmpty;
    // 线程池中线程ids
    pthread_t* m_threadIDs;
    // 管理折线程
    pthread_t m_managerID;
    // 任务列表
    TaskQueue<T>* m_taskQ;
    // 最小线程数量
    int m_minNum;
    // 最大线程数量
    int m_maxNum;
    // 正在工作的线程数量
    int m_busyNum;
    // 存活的线程数量
    int m_aliveNum;
    // 要销毁的线程数量
    int m_exitNum;
    // 是否销毁线程池
    bool m_shutdown = false;

    static const int NUMBER = 2;


};