#include "04.ThreadPool.h"
#include "04.ThreadPool.cpp"
#include <iostream>
#include <string.h> // memset
#include <string>
#include <unistd.h>
using namespace std;

template <typename T>
ThreadPool<T>::ThreadPool(int min, int max)
{
    do
    {
        // 实例化任务队列
        m_taskQ = new TaskQueue<T>;

        if (m_taskQ == nullptr)
        {
            cout << "taskQ init fail." << endl;
            break;
        }
        

        // 为threadids 分配空间
        m_threadIDs=new pthread_t[max];
        if (m_threadIDs == nullptr)
        {
            cout << "threadIDS INIT FAIL." << endl;
            break;
        }
        // 初始化threadids 为 0 
        memset(m_threadIDs, 0, sizeof(pthread_t)*max);
        
        m_minNum = min;
        m_maxNum = max;
        m_busyNum = 0;
        m_aliveNum = 0; // 和线程最小个数相同
        m_exitNum = 0;

        if (pthread_mutex_init(&m_lock, NULL) != 0 ||
            pthread_cond_init(&m_notEmpty, NULL) !=0)
        {
            cout << "mutex or condition INIT FAIL." << endl;
            break;
        }

        m_shutdown = false;

        // 创建管理者线程
        // 将this传递进去是因为，manager是一个静态成员函数，它只能够访问静态成员属性，因此将this当作参数传递进去，使manager也可以访问其非静态成员变量和非静态成员函数
        pthread_create(&m_managerID, NULL, manager, this);
        // 工作线程
        for(int i = 0; i < min; ++i)
        {
            pthread_create(&m_threadIDs[i], NULL, worker, this);
        }

        return; // 增加return，当程序执行到这里的时候，表明初始化完成，直接跳出构造函数后面所有的代码
    } while (0);

    // 上面执行的出现问题，跳出循环，则直接开始回收资源
    if(m_threadIDs) delete[] m_threadIDs;
    if(m_taskQ) delete m_taskQ;
    
}

template <typename T>
ThreadPool<T>::~ThreadPool()
{
    // 关闭线程池
    m_shutdown = 1;
    // 阻塞回收管理者线程
    pthread_join(m_managerID, NULL);
    // 唤醒阻塞的消费者
    for (int i = 0; i < m_aliveNum; ++i)
    {
        pthread_cond_signal(&m_notEmpty); // 唤醒后就会自动退出，见worker阻塞操作后的内容
    }

    // 释放pool申请的堆内存
    // 任务队列
    if (m_taskQ != NULL)
    {
        delete m_taskQ;
    }

    if(m_threadIDs!=NULL){
       delete[] m_threadIDs;
    }


    // 销毁互斥锁
    pthread_mutex_destroy(&m_lock);
    // 销毁条件变量
    pthread_cond_destroy(&m_notEmpty);

}

template <typename T>
void ThreadPool<T>::addTask(Task<T> task)
{
    // pthread_mutex_lock(&m_lock); // 加锁 
    // 因为核心代码，addtask已经加锁了，所以这里不用加了
    
    if (m_shutdown)
    {
        // pthread_mutex_unlock(&m_lock); // 解锁
        return;
    }
    
    // 添加任务
    m_taskQ->addTask(task);

    // 唤醒线程，生产者已经添加任务了，唤醒因为没有任务而堵塞的消费者
    pthread_cond_signal(&m_notEmpty);

    // pthread_mutex_unlock(&m_lock); // 解锁
}

template <typename T>
int ThreadPool<T>::getBusyNumber()
{
    pthread_mutex_lock(&m_lock); // 加锁
    int busyNum = this->m_busyNum;
    pthread_mutex_unlock(&m_lock); // 解锁
    return busyNum;
}

template <typename T>
int ThreadPool<T>::getAliveNumber()
{
    pthread_mutex_lock(&m_lock); // 加锁
    int aliveNum = this->m_aliveNum;
    pthread_mutex_unlock(&m_lock); // 解锁
    return aliveNum;
}

template <typename T>
void *ThreadPool<T>::worker(void *arg)
{
    ThreadPool<T>* pool = static_cast<ThreadPool<T> *> (arg); // 将传递参数做类型转换
    while (true)
    {
        // 因为worker是一个静态成员函数，他无法访问非静态成员变量，因此要使用传递进来的参数访问非静态成员变量
        pthread_mutex_lock(&pool->m_lock);

        // 如果线程池中的任务列表数量为0，并且线程池不是要关闭
        while (pool->m_taskQ->taskNumber() == 0 && !pool->m_shutdown)
        {
            // 阻塞工作线程
            pthread_cond_wait(&pool->m_notEmpty, &pool->m_lock); // 条件变量地址，互斥锁地址

            // 被唤醒后，判断是否要杀死线程
            // 如果要退出的线程数， exitnum大于0 说明要杀死线程
            if(pool->m_exitNum>0){
                // 减少一个杀死的数量
                --pool->m_exitNum;
                // 如果活着的线程数大于线程池最小的线程数，那么才删除，否则就保持当前线程状态
                if(pool->m_aliveNum > pool->m_minNum){
                    --pool->m_aliveNum;
                    // 杀死线程后，解锁，防止死锁
                    pthread_mutex_unlock(&pool->m_lock);
                    pool->threadExit();
                }
            }
        }

        // 判断线程池是否关闭
        // 如果线程池关闭，则退出当前线程
        if(pool->m_shutdown){
            pthread_mutex_unlock(&pool->m_lock);
            pool->threadExit();
        }

        // 从任务列表中取出任务
        Task<T> task = pool->m_taskQ->takeTask();

        // 工作线程数量++
        ++pool->m_busyNum;

        // 解锁
        pthread_mutex_unlock(&pool->m_lock);

        cout << "thread " << to_string(pthread_self()) << " start working ."  << endl;

        // 执行任务函数
        task.function(task.arg);
        // 释放任务函数所占用的资源
        delete task.arg;
        task.arg=nullptr;

        cout << "thread " << to_string(pthread_self()) << " end working ."  << endl;
        pthread_mutex_lock(&pool->m_lock);
        --pool->m_busyNum;
        pthread_mutex_unlock(&pool->m_lock);
    }
    
    return nullptr;
}

template <typename T>
void *ThreadPool<T>::manager(void *arg)
{
    ThreadPool<T>* pool = static_cast<ThreadPool<T> *> (arg); // 将传递参数做类型转换
    while (!pool->m_shutdown)
    {
        sleep(3);

        pthread_mutex_lock(&pool->m_lock);
        int queueSize = pool->m_taskQ->taskNumber();
        int liveNum = pool->m_aliveNum;
        int busyNum = pool->m_busyNum;
        pthread_mutex_unlock(&pool->m_lock);

        // 添加和销毁线程
        // 添加，自定义条件，根据实际业务需求设计
        // 当前任务的个数>存活线程数 && 存活线程数<最大线程数
        if(queueSize>liveNum && liveNum<pool->m_maxNum){
             int count=0;
             pthread_mutex_lock(&pool->m_lock); // 加锁，因为要修改liveNum的值
             for (int i = 0; i < pool->m_maxNum && count<NUMBER && pool->m_aliveNum<pool->m_maxNum; ++i)
             {
                if(pool->m_threadIDs[i] == 0){ // 如果是0，就说明当前没有存储线程id
                    pthread_create(&pool->m_threadIDs[i], NULL, worker, pool);
                    ++count;
                    ++pool->m_aliveNum;  
                }
                
             }
             pthread_mutex_unlock(&pool->m_lock); // 解锁
             
        }

        // 销毁 自定义条件，根据实际业务需求设计
        // 忙的线程*2 < 存货的线程数 && 存活线程数>最小线程数
        if (busyNum*2<liveNum && liveNum>pool->m_minNum){
            pthread_mutex_lock(&pool->m_lock); // 加锁
            pool->m_exitNum = NUMBER; // 要销毁的线程数            
            pthread_mutex_unlock(&pool->m_lock); // 解锁
 
            // 让工作线程自杀
            // 管理者线程不能直接杀死空闲线程，因为它不知道谁是空闲线程，只能通知空闲线程让它自杀
            for (int i = 0; i < NUMBER; ++i)
            {
                pthread_cond_signal(&pool->m_notEmpty); // 唤醒被阻塞的线程，所以调用两次阻塞线程唤醒，然后让唤醒的2个线程自己去自杀，实现销毁进程的目的。见126行
            }
            
        }

    }
    
    return nullptr;
}

template <typename T>
void ThreadPool<T>::threadExit()
{
    pthread_t tid = pthread_self();
    for (int i = 0; i < m_maxNum; ++i)
    {
        // 当threadids中保存的tid和当前调用threadexit函数的进程id相同，就将其修改为0，并退出当前线程
        if(m_threadIDs[i] == tid){
            m_threadIDs[i] = 0;
            cout << "threadExit() called, " << to_string(tid) << " exiting." << endl;
            break;
        }
    }
    pthread_exit(NULL);
}
