#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For usleep()

pthread_cond_t cond; // 消费者条件变量，当商品数量为0，就阻塞消费者线程，直到生产者生产完商品通知消费者来消费
pthread_cond_t cond2; // 生产者条件变量，当商品数量为5，就阻塞生产者线程，直到消费者消费了商品，通知生产者补货
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
        // 增加互斥锁，因为不能允许多个进程在同一时间对链表进行增加
        pthread_mutex_lock(&mutex);

        while (count == 5)
        {
            pthread_cond_wait(&cond2, &mutex);
            printf("++商品数量上限了，阻塞生产者 \n");
        }
        
        // 创建新节点
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        // 初始化节点
        newNode->num = rand()%1000;
        newNode->next = head;
        head = newNode;
        printf("生产者， id=%ld, num=%d \n", pthread_self(), newNode->num);

        // 节点数量+1；
        ++count;
        printf("当前商品数量： %d \n", count);
        // 增加节点结束后，解锁
        pthread_mutex_unlock(&mutex);

        // 生产结束后，使用广播通知条件变量，可以开始消费了
        pthread_cond_broadcast(&cond); // 因为cond中保存了是哪一个消费者进行阻塞，所以通过这个可以直接通知到对应线程

        // 随即进行休眠
        sleep(rand()%3);
    }
    
    return NULL;
}

// 消费者函数
void * consumer(void * args){
    while (1)
    {
        // 因为要删除头节点，所以也是写操作，要增加锁
        pthread_mutex_lock(&mutex);

        // 消费者不能够无休止的删除节点，因为生产者可能还没有生产出来
        while (head == NULL)
        { // 如果头节点为空，说明现在没有商品，需要阻塞当前进程，等待生产者生产商品才能够消费
            pthread_cond_wait(&cond, &mutex);
            printf("--没有商品了，阻塞消费者 \n");
            // 虽然程序在48行的时候上锁成功，但是pthread_cond_wait程序一旦被调用，首先会阻塞线程，然后将拥有的互斥锁打开，这是在pthread_cond_wait函数内部实现的
            // 所以当上锁后发现没有商品，消费者会解锁，然后直到生产者抢到互斥锁开始做生产，生产结束后广播通知所有的消费者，然后消费者再抢互斥锁，抢到互斥锁的才会向下执行删除头节点的操作
        }
        
        // 取出头节点
        struct Node* node = head;
        printf("消费者， id=%ld, num=%d \n", pthread_self(), node->num);
        // 改变头节点指向
        head=head->next;
        // 释放头节点空间
        free(node); 
        
        // 节点数量减减
        --count;
        printf("当前商品数量： %d \n", count);
        //解锁
        pthread_mutex_unlock(&mutex);
        
        //消费结束后，使用广播通知条件变量，可以开始生产补货了
        pthread_cond_broadcast(&cond2); // 因为cond中保存了是哪一个生产者进行阻塞，所以通过这个可以直接通知到对应线程

        // 随即进行休眠
        sleep(rand()%3);
    }
    
    return NULL;
}

int main(){
    // 条件变量，
    /*
    严格意义上来说，条件变量的主要作用不是处理线程同步, 而是进行线程的阻塞。
    如果在多线程程序中只使用条件变量无法实现线程的同步, 必须要配合互斥锁来使用。
    虽然条件变量和互斥锁都能阻塞线程，但是二者的效果是不一样的，二者的区别如下：
        1. 假设有A-Z 26个线程，这26个线程共同访问同一把互斥锁，如果线程A加锁成功，那么其余B-Z线程访问互斥锁都阻塞，
            所有的线程只能顺序访问临界区
        2. 条件变量只有在满足指定条件下才会阻塞线程，如果条件不满足，多个线程可以同时进入临界区，同时读写临界资源，
            这种情况下还是会出现共享资源中数据的混乱。

    一般情况下条件变量用于处理[生产者和消费者模型]，并且和互斥锁配合使用。
    pthread_cond_t cond; // 被条件变量阻塞的线程的线程信息会被记录到这个变量中，以便在解除阻塞的时候使用。

    // 初始化
    int pthread_cond_init(pthread_cond_t *restrict cond,
        const pthread_condattr_t *restrict attr);
    // 销毁释放资源        
    int pthread_cond_destroy(pthread_cond_t *cond);
    参数:
        cond: 条件变量的地址
        attr: 条件变量属性, 一般使用默认属性, 指定为NULL

    // 线程阻塞函数, 哪个线程调用这个函数, 哪个线程就会被阻塞
    int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
    通过函数原型可以看出，该函数在阻塞线程的时候，需要一个互斥锁参数，这个互斥锁主要功能是进行线程同步，让线程顺序进入临界区，避免出现数共享资源的数据混乱。该函数会对这个互斥锁做以下几件事情：

        1.在阻塞线程时候，如果线程已经对互斥锁mutex上锁，那么会将这把锁 打开，这样做是为了避免死锁
        2.当线程解除阻塞的时候，函数内部会帮助这个线程再次将这个mutex互斥锁锁上，继续向下访问临界区

    // 表示的时间是从1971.1.1到某个时间点的时间, 总长度使用秒/纳秒表示
    struct timespec {
        time_t tv_sec;      // Seconds 
        long   tv_nsec;     // Nanoseconds [0 .. 999999999] 
    };
    // 将线程阻塞一定的时间长度, 时间到达之后, 线程就解除阻塞了
    int pthread_cond_timedwait(pthread_cond_t *restrict cond,
            pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
    这个函数的前两个参数和pthread_cond_wait函数是一样的，第三个参数表示线程阻塞的时长，
        但是需要额外注意一点：struct timespec这个结构体中记录的时间是从1971.1.1到某个时间点的时间，总长度使用秒/纳秒表示。因此赋值方式相对要麻烦一点：
    time_t mytim = time(NULL);	// 1970.1.1 0:0:0 到当前的总秒数
    struct timespec tmsp;
    tmsp.tv_nsec = 0;
    tmsp.tv_sec = time(NULL) + 100;	// 线程阻塞100s

    // 唤醒阻塞在条件变量上的线程, 至少有一个被解除阻塞
    int pthread_cond_signal(pthread_cond_t *cond);
    // 唤醒阻塞在条件变量上的线程, 被阻塞的线程全部解除阻塞
    int pthread_cond_broadcast(pthread_cond_t *cond);
    调用上面两个函数中的任意一个，都可以换线被pthread_cond_wait或者pthread_cond_timedwait阻塞的线程，
        区别就在于pthread_cond_signal是唤醒至少一个被阻塞的线程（总个数不定），
                 pthread_cond_broadcast是唤醒所有被阻塞的线程。

    */

    /* 生产者消费者模型
        生产者和消费者模型的组成：

        生产者线程 -> 若干个
            生产商品或者任务放入到任务队列中
            任务队列满了就阻塞, 不满的时候就工作
            通过一个生产者的条件变量控制生产者线程阻塞和非阻塞
        消费者线程 -> 若干个
            读任务队列, 将任务或者数据取出
            任务队列中有数据就消费，没有数据就阻塞
            通过一个消费者的条件变量控制消费者线程阻塞和非阻塞
        队列 -> 存储任务/数据，对应一块内存，为了读写访问可以通过一个数据结构维护这块内存
            可以是数组、链表，也可以使用stl容器：queue / stack / list / vector

       只需要考虑这样的条件就够了： 如果队列是满的，就阻塞生产者，如果是空的，就阻塞消费者；
    
    */
    // 示例，使用条件变量实现生产者和消费者模型，生产者5个，往链表中增加节点，消费者5个，删除链表头节点

    // 初始化
    pthread_mutex_init(&mutex, NULL); // 互斥锁的地址，互斥锁的属性
    pthread_cond_init(&cond, NULL); // 条件变量的地址，条件变量的属性

    // 生产者ids和消费者线程t2
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
    pthread_cond_destroy(&cond); // 销毁条件变量


    /*
        要注意，有多少个条件就要有多少个条件信号。
        对应到这里，生产者会判断是否到达上限，这是一个信号；消费者要判断是否还有商品来消费，这也是一个信号；

        当消费者发现没有商品了，就要阻塞消费者的线程，直到生产者生产完商品，通知消费者来消费。
        当生产者发现商品到上限了，就要阻塞生产者线程，直到消费者消耗商品，通知生产者来生产补货。
        ！两者是相互的
    */
    
    return 0;
}