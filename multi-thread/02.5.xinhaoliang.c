#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For usleep()
#include <semaphore.h>

// 生产者信号量
sem_t semp;
// 消费者信号量
sem_t semc;

pthread_mutex_t mutex; // 互斥锁

// 链表节点
struct Node{
    int num;
    struct Node* next;
};
// 链表头节点
struct Node* head=NULL;
// 链表结点数量
int count = 0;

// 生产者函数
void * producer(void* args){
    while (1)
    {
        // 生产之前，判断有没有资源
        sem_wait(&semp); // 如果没有资源，就阻塞生产者

        pthread_mutex_lock(&mutex); // 上锁，一定要放在阻塞线程后面，不然会有可能出现死锁
        
        // 创建新节点
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        // 初始化节点
        newNode->num = rand()%1000;
        newNode->next = head;
        head = newNode;
        printf("生产者， id=%ld, num=%d \n", pthread_self(), newNode->num);

        pthread_mutex_unlock(&mutex); // 解锁

        // 生产过后，消费者资源+1，进而不再阻塞消费者进程
        sem_post(&semc); // 

        // 随即进行休眠
        sleep(rand()%3);
    }
    
    return NULL;
}

// 消费者函数
void * consumer(void * args){
    while (1)
    {
        // 消费前，判断是否有资源，没有资源就阻塞消费者
        sem_wait(&semc);
        
        pthread_mutex_lock(&mutex); // 上锁，一定要放在阻塞线程后面，不然会出现死锁

        // 取出头节点
        struct Node* node = head;
        printf("消费者， id=%ld, num=%d \n", pthread_self(), node->num);
        // 改变头节点指向
        head=head->next;
        // 释放头节点空间
        free(node); 

        pthread_mutex_unlock(&mutex); // 解锁

        sem_post(&semp);

        // 随即进行休眠
        sleep(rand()%3);
    }
    
    return NULL;
}

int main(){
    // 信号量
    // 信号量和条件变量一样是用于处理生产者和消费者模型，用于阻塞生产者线程或者消费者线程的运行。信号的类型为sem_t对应的头文件为<semaphore.h>：
    // 信号量（信号灯）与互斥锁和条件变量的主要不同在于”灯”的概念，灯亮则意味着资源可用，灯灭则意味着不可用。
    // 信号量主要阻塞线程, 不能完全保证线程安全，如果要保证线程安全, 需要信号量和互斥锁一起使用。
    /*
        sem_t sem;

        // 初始化信号量/信号灯
        int sem_init(sem_t *sem, int pshared, unsigned int value);
        // 资源释放, 线程销毁之后调用这个函数即可
        // 参数 sem 就是 sem_init() 的第一个参数            
        int sem_destroy(sem_t *sem);
        参数:
            sem：信号量变量地址
            pshared：
                0：线程同步
                非0：进程同步
            value：初始化当前信号量拥有的资源数（>=0），如果资源数为0，线程就会被阻塞了。

        // 参数 sem 就是 sem_init() 的第一个参数  
        // 函数被调用sem中的资源就会被消耗1个, 资源数-1
        int sem_wait(sem_t *sem);
        当线程调用这个函数，并且sem中的资源数>0，线程不会阻塞，线程会占用sem中的一个资源，因此资源数-1，
        直到sem中的资源数减为0时，资源被耗尽，因此线程也就被阻塞了。

        // 参数 sem 就是 sem_init() 的第一个参数  
        // 函数被调用sem中的资源就会被消耗1个, 资源数-1
        int sem_trywait(sem_t *sem);
        当线程调用这个函数，并且sem中的资源数>0，线程不会阻塞，线程会占用sem中的一个资源，因此资源数-1，
        直到sem中的资源数减为0时，资源被耗尽，但是线程不会被阻塞，直接返回错误号，因此可以在程序中添加判断分支，用于处理获取资源失败之后的情况。


        // 表示的时间是从1971.1.1到某个时间点的时间, 总长度使用秒/纳秒表示
        struct timespec {
            time_t tv_sec;      // Seconds 
            long   tv_nsec;     // Nanoseconds [0 .. 999999999] 
        };
        // 调用该函数线程获取sem中的一个资源，当资源数为0时，线程阻塞，在阻塞abs_timeout对应的时长之后，解除阻塞。
        // abs_timeout: 阻塞的时间长度, 单位是s, 是从1970.1.1开始计算的
        int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
        该函数的参数abs_timeout和pthread_cond_timedwait的最后一个参数是一样的，使用方法不再过多赘述。
        当线程调用这个函数，并且sem中的资源数>0，线程不会阻塞，线程会占用sem中的一个资源，因此资源数-1，
        直到sem中的资源数减为0时，资源被耗尽，线程被阻塞，当阻塞指定的时长之后，线程解除阻塞。

        // 调用该函数给sem中的资源数+1
        int sem_post(sem_t *sem);
        调用该函数会将sem中的资源数+1，
        如果有线程在调用sem_wait、sem_trywait、sem_timedwait时因为sem中的资源数为0被阻塞了，这时这些线程会解除阻塞，获取到资源之后继续向下运行。

        // 查看信号量 sem 中的整形数的当前值, 这个值会被写入到sval指针对应的内存中
        // sval是一个传出参数
        int sem_getvalue(sem_t *sem, int *sval);
        通过这个函数可以查看sem中现在拥有的资源个数，通过第二个参数sval将数据传出，也就是说第二个参数的作用和返回值是一样的。

    */


    // ！ 资源为1的时候  ！ 不涉及线程同步的问题，因为资源为1时，每次只有一个线程会工作，所以可以不加锁
    // 生产者初始化
    sem_init(&semp, 0, 5);// 信号量地址，同步，资源数
    // 消费者初始化 刚开始不给消费，初始化0， 所以消费者线程启动的时候就阻塞了
    sem_init(&semc, 0, 0);
    // 初始化互斥锁
    pthread_mutex_init(&mutex, NULL); // 互斥锁的地址，互斥锁的属性

    // 生产者ids和消费者线程ids
    pthread_t t1[5], t2[5];

    // 创建子线程
    // 生产者
    for (int i = 0;i < 5;++i)
    {
        pthread_create(&t1[i], NULL, producer, NULL); // id，线程属性，线程执行函数，给函数传递的参数
    }

    // 消费者
    for (int i = 0;i < 5;++i)
    {
        pthread_create(&t2[i], NULL, consumer, NULL); // id，线程属性，线程执行函数，给函数传递的参数
    }
    
    // 回收资源
    // 生产者
    for (int i = 0;i < 5;++i)
    {
        pthread_join(&t1[i], NULL); // 线程id， 接收的数据
    }
    // 消费者
    for (int i = 0;i < 5;++i)
    {
        pthread_join(&t2[i], NULL); // 线程id， 接收的数据
    }


    // 销毁
    pthread_mutex_destroy(&mutex); // 先销毁互斥锁
    // 销毁信号量
    sem_destroy(&semp);
    sem_destroy(&semc);
    
    return 0;
}