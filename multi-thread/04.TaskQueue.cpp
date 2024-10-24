#include "04.TaskQueue.h"

template <class T>
TaskQueue<T>::TaskQueue()
{
    // 初始化互斥锁
    pthread_mutex_init(&m_mutex, NULL); // 互斥锁地址，锁的属性
}

template <class T>
TaskQueue<T>::~TaskQueue()
{
    // 销毁互斥锁
    pthread_mutex_destroy(&m_mutex); // 互斥锁地址
}

template <class T>
void TaskQueue<T>::addTask(Task<T> task)
{
    // 加锁
    pthread_mutex_lock(&m_mutex);

    // 把新任务添加到队列中
    m_taskQ.push(task);

    // 解锁
    pthread_mutex_unlock(&m_mutex);

}

template <class T>
void TaskQueue<T>::addTask(callback f, void *arg)
{
    
    // 加锁
    pthread_mutex_lock(&m_mutex);

    // 把新任务添加到队列中
    m_taskQ.push(Task<T>(f, arg));

    // 解锁
    pthread_mutex_unlock(&m_mutex);
}

template <class T>
Task<T> TaskQueue<T>::takeTask()
{
    Task<T> t;
    // 加锁
    pthread_mutex_lock(&m_mutex);
    // 判断任务队列是否为空
    if(!m_taskQ.empty()){
        t = m_taskQ.front();
        m_taskQ.pop();
    }
    // 解锁
    pthread_mutex_unlock(&m_mutex);
    return t;
}
