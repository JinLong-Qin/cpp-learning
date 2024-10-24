#include "03.threadpool.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int NUMBER = 2; // 要添加线程的时候，每次添加多少个线程

// 任务结构体
typedef struct Task{
    void (*function)(void *arg); // void*是一个泛型，可以传递任意类型的数据
    void * arg;
} Task;

// 线程池结构体 ；多个任务的集合
struct ThreadPool
{
    // 任务队列
    Task * taskQ;
    int queueCapacity; // 容量
    int queueSize; // 当前的任务数量
    int queueFront; // 队头 - > 取数据
    int queueRear; // 队尾 -> 放数据

    // 管理者线程 只有一个
    pthread_t mamageID;

    // 工作线程 多个
    pthread_t * threadIDs;
    int minNum; // 最小线程数
    int maxNum; // 最多线程数
    int busyNum; // 正在工作的线程数
    int liveNum; // 现在存活的线程数
    int exitNum; // 要杀死的线程数

    pthread_mutex_t mutexPool; // 整个线程池的锁
    pthread_mutex_t mutexBusy; // busyNum的锁,因为只要有线程在动或者不动，busyNum都要有变化

    pthread_cond_t isFull; // 条件变量，判断任务队列是否满了
    pthread_cond_t isEmpty; // 条件变量，判断任务队列是否空了

    int shutdown; // 是不是要销毁线程池，销毁1 不销毁0

};


// 线程池的创建和初始化
ThreadPool* threadPool_create(int minNum, int maxNum, int queuesCapacity){
    // 为线程池分配空间
    ThreadPool* pool = (ThreadPool*) malloc(sizeof(ThreadPool));
    do
    {
        if(pool == NULL){ // 如果分配的空间为空，说明分配失败
            printf("malloc threadpool fail.");
            // return NULL;
            break;
        }
        // 为运行id分配空间，初始化为最大的进程数量所占的空间
        pool->threadIDs = (pthread_t*)malloc(sizeof(pthread_t) * maxNum);
        if (pool->threadIDs == NULL) // 如果分配的空间为空，说明分配失败
        {
            printf("malloc threadIDs fail.");
            // return NULL;
            break;
        }
        memset(pool->threadIDs, 0, sizeof(pthread_t) * maxNum); // 为这部分内存初始化为0，后期如果，检测到值不为0说明存在进程id

        // 其他属性设置
        pool->minNum=minNum;
        pool->maxNum=maxNum;
        pool->busyNum=0;
        pool->liveNum=minNum;  // 设置活跃进程数和最小个数相同
        pool->exitNum=0;

        // 锁初始化
        if( pthread_mutex_init(&pool->mutexPool, NULL)!=0||
            pthread_mutex_init(&pool->mutexBusy, NULL)!=0||
            pthread_cond_init(&pool->isFull, NULL) !=0 ||
            pthread_cond_init(&pool->isEmpty, NULL)!=0)
        { // 返回值如果不为0，说明初始化失败
            printf("mutex, cond init fail");
            // return NULL;
            break;
        }

        // 任务队列
        pool->taskQ = (Task *)malloc(sizeof(Task) * queuesCapacity);
        pool->queueCapacity = queuesCapacity;
        pool->queueSize = 0;
        pool->queueFront=0;
        pool->queueRear=0;

        // 线程池标志
        pool->shutdown=0;

        // 创建线程
        // 管理者线程
        pthread_create(&pool->mamageID, NULL, manager, pool);
        // 子线程
        for (int i = 0; i < minNum; ++i)
        {
            pthread_create(&pool->threadIDs[i], NULL, worker, pool); // 对于工作线程，每次的任务是从taskQ中取任务做执行，所以将pool作为参数传递进去
        }

        return pool;
    } while (0); // 使用循环的好处是可以使用break，
    // 如果break了，就会跳出循环，则这里直接释放资源

    // 释放资源
    if(pool && pool->threadIDs) free(pool->threadIDs);
    if(pool && pool->taskQ) free(pool->taskQ);
    if(pool) free(pool);
    
    
    return NULL;
}

int threadPoolDestory(ThreadPool *pool)
{
    // 判断pool是否是一个有效地址
    if(pool == NULL){ // 不是有效地址就不管
        return -1;
    }
    // 否则就销毁线程池
    pool->shutdown = 1;
    // 阻塞回收管理者线程
    pthread_join(pool->mamageID, NULL);
    // 唤醒阻塞的消费者
    for (int i = 0; i < pool->liveNum; ++i)
    {
        pthread_cond_signal(&pool->isEmpty); // 唤醒后就会自动退出，见worker阻塞操作后的内容
    }

    // 释放pool申请的堆内存
    // 任务队列
    if (pool->taskQ != NULL)
    {
        free(pool->taskQ);
    }

    if(pool->threadIDs!=NULL){
        free(pool->threadIDs);
    }


    // 销毁互斥锁
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_mutex_destroy(&pool->mutexBusy);
    // 销毁条件变量
    pthread_cond_destroy(&pool->isEmpty);
    pthread_cond_destroy(&pool->isFull);

    free(pool);
    pool = NULL;
    
    
    return 0;
}

void threadPoolAdd(ThreadPool* pool, void(*func) (void*), void* arg){
    pthread_mutex_lock(&pool->mutexPool); // 加锁
    while (pool->queueSize == pool->queueCapacity && pool->shutdown)
    {
        // 阻塞生产者进程 即向线程池中增加任务的
        pthread_cond_wait(&pool->isFull, &pool->mutexPool);
    }
    if (pool->shutdown)
    {
        pthread_mutex_unlock(&pool->mutexPool); // 解锁
        return;
    }
    // 添加任务
    pool->taskQ[pool->queueRear].function = func;
    pool->taskQ[pool->queueRear].arg = arg;
    pool->queueRear = (pool->queueRear+1) % pool->queueCapacity; // 移动尾指针
    ++pool->queueSize;

    // 唤醒线程，生产者已经添加任务了，唤醒因为没有任务而堵塞的消费者
    pthread_cond_signal(&pool->isEmpty);
    

    pthread_mutex_unlock(&pool->mutexPool); // 解锁
}

int threadPoolBusyNum(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->mutexBusy);
    int busyNum = pool->busyNum;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNum;
}

int threadPoolLiveNum(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->mutexPool);
    int liveNum = pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);
    return liveNum;
}

void* worker(void* arg){
    ThreadPool* pool = (ThreadPool*) arg; // 将参数转换数据类型
    while (1) // 不断的去在任务队列中取任务
    {
        pthread_mutex_lock(&pool->mutexPool); // 加锁

        while(pool->queueSize == 0 && !pool->shutdown){ // 如果线程池没有关闭，并且任务队列同时为空
            // 阻塞工作线程
            pthread_cond_wait(&pool->isEmpty, &pool->mutexPool);

            // 判断是否要销毁线程
            if(pool->exitNum > 0){ // 当线程被唤醒后，发现要退出的线程数量不为0，就主动自己去自杀
                --pool->exitNum;
                if (pool->liveNum > pool->minNum)
                {
                    --pool->liveNum;
                    pthread_mutex_unlock(&pool->mutexPool); // 解锁，防止死锁
                    // pthread_exit(NULL);
                    threadExit(pool);
                }
            }
        }

        // 判断线程池是否关闭
        if(pool->shutdown){ // 如果被关闭了，就把锁打开（防止死锁），同时退出线程
            pthread_mutex_unlock(&pool->mutexPool); // 解锁
            // pthread_exit(NULL);
            threadExit(pool);
        }

        // 从任务队列中取出任务
        // 从任务队列头开始取任务，
        Task task;
        task.function=pool->taskQ[pool->queueFront].function;
        task.arg = pool->taskQ[pool->queueFront].arg;

        // 移动队列头，因为现在的头已经被取出去了
        // 循环队列的思想，移动头的位置
        pool->queueFront = (pool->queueFront+1)%pool->queueCapacity;

        // 已经取出一个任务了，所以池子中的任务数量-1
        --pool->queueSize;

        pthread_cond_signal(&pool->isFull); // 发送信号，唤醒因为任务队列满了而阻塞的生产者，因为消费者已经消费了任务，可以继续向里面添加任务了

        pthread_mutex_unlock(&pool->mutexPool); // 解锁

        
        printf("thread %ld is start working \n", pthread_self());
        // 取出任务要执行，然后需要对工作线程数量+1
        pthread_mutex_lock(&pool->mutexBusy);
        ++pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

        // 执行任务
        task.function(task.arg);

        // free(task.arg);
        // task.arg=NULL;

        printf("thread %ld is end working \n", pthread_self());
        // 执行完要对工作线程数量-1
        pthread_mutex_lock(&pool->mutexBusy);
        --pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);






    }
    
    return NULL;
}


void* manager(void* arg){
    ThreadPool* pool = (ThreadPool*)arg;
    while(!pool->shutdown){
        // 每隔三秒钟检测一次
        sleep(3);

        // 取出线程池中任务数量和线程数量
        pthread_mutex_lock(&pool->mutexPool); // 加锁
        int queueSize = pool->queueSize;
        int liveNum = pool->liveNum;


        pthread_mutex_unlock(&pool->mutexPool); // 解锁

        // 取出正在工作的线程数量
        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

        // 添加和销毁线程
        // 添加，自定义条件，根据实际业务需求设计
        // 当前任务的个数>存活线程数 && 存活线程数<最大线程数
        if(queueSize>liveNum && liveNum<pool->maxNum){
             int count=0;
             pthread_mutex_lock(&pool->mutexPool); // 加锁，因为要修改liveNum的值
             for (int i = 0; i < pool->maxNum && count<NUMBER && pool->liveNum<pool->maxNum; ++i)
             {
                if(pool->threadIDs[i] == 0){ // 如果是0，就说明当前没有存储线程id
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);
                    ++count;
                    ++pool->liveNum;  
                }
                
             }
             pthread_mutex_unlock(&pool->mutexPool); // 解锁
             
        }

        // 销毁 自定义条件，根据实际业务需求设计
        // 忙的线程*2 < 存货的线程数 && 存活线程数>最小线程数
        if (busyNum*2<liveNum && liveNum>pool->minNum){
            pthread_mutex_lock(&pool->mutexPool); // 加锁
            pool->exitNum = NUMBER; // 要销毁的线程数            
            pthread_mutex_unlock(&pool->mutexPool); // 解锁

            // 让工作线程自杀
            // 管理者线程不能直接杀死空闲线程，因为它不知道谁是空闲线程，只能通知空闲线程让它自杀
            for (int i = 0; i < NUMBER; ++i)
            {
                pthread_cond_signal(&pool->isEmpty); // 唤醒被阻塞的线程，所以调用两次阻塞线程唤醒，然后让唤醒的2个线程自己去自杀，实现销毁进程的目的。见126行
            }

            
            
        }
        

    }

    return NULL;
}

void threadExit(ThreadPool* pool){
    pthread_t tid = pthread_self();
    for (int i = 0; i < pool->maxNum; ++i)
    {
        if(pool->threadIDs[i] == tid){
            pool->threadIDs[i]=0;
            printf("threadExit() called, %ld exiting \n", tid);
            break;
        }
    }
    pthread_exit(NULL);
}