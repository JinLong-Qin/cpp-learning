#pragma once
#include <queue>
#include <pthread.h>

using namespace std;
using callback = void (*) (void* arg); // c++11特性，using

// 任务结构体
template<typename T>
struct Task<T>{
    // void (*function) (void* arg); // 存储的是任务函数的地址，所以是函数指针类型

    // c++中 stuct也是类，所以可以添加构造函数
    Task<T>(){ // 无参构造函数
        function = nullptr; // 给指针赋初值，用nullptr
        arg = nullptr;
    }

    Task<T>(callback f, void* arg){ // 有参构造函数
        this->arg = (T*)arg;
        function=f;
    }

    callback function;
    T* arg; // 存储任务函数要执行时的参数
};

template<typename T>
class TaskQueue<T>{

public:
    TaskQueue<T>();

    ~TaskQueue<T>();

    // 添加任务
    void addTask(Task<T> task);
    void addTask(callback f, void* arg);
    // 取出任务
    Task<T> takeTask();
    // 获取当前任务个数
    inline size_t taskNumber(){ // 内联函数
        // 特点是：内联函数不会压栈，直接进行代码块的替换，效率更高，如果函数内部只有少数代码，并且没有任何的判定，就可以使用内联函数
        // 但是是否内联还是要看编译器自己做决定
        return m_taskQ.size();
    }

private:
    pthread_mutex_t m_mutex;
    queue<Task<T>> m_taskQ; // 任务队列

};