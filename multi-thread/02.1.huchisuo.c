#include <stdio.h>
#include <pthread.h>

#define MAX 50
int number; // 记录当前数数的值

pthread_mutex_t mutex; // 创建互斥锁对象，全局变量保证其在运行期间不会被释放掉

void * func_A(void * args);
void * func_B(void * args);
int main(){
    // 互斥锁
    
    pthread_t p1, p2;
    pthread_mutex_init(&mutex, NULL); // 互斥锁初始化。（互斥锁地址，互斥锁属性（使用默认属性所以给NULL））
    pthread_create(&p1, NULL, func_A, NULL);
    pthread_create(&p2, NULL, func_B, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    pthread_mutex_destroy(&mutex); // 销毁互斥锁
    
    // 所谓的共享资源就是多个线程共同访问的变量，这些变量通常为全局数据区变量或者堆区变量，这些变量对应的 [共享资源也被称之为临界资源]。
    /*
    找到临界资源之后，再找和[临界资源相关的上下文代码]，这样就得到了一个代码块，这个[代码块可以称之为临界区]。确定好临界区（临界区越小越好）之后，就可以进行线程同步了，线程同步的大致处理思路是这样的：
        1.在临界区代码的上边，添加加锁函数，对临界区加锁。
            哪个线程调用这句代码，就会把这把锁锁上，其他线程就只能阻塞在锁上了。
        2.在临界区代码的下边，添加解锁函数，对临界区解锁。
            出临界区的线程会将锁定的那把锁打开，其他抢到锁的线程就可以进入到临界区了。
        3.通过锁机制能保证临界区代码最多只能同时有一个线程访问，这样并行访问就变为串行访问了。
    */

    /*
        互斥锁：特点是，这个锁只能够被一个线程使用，锁的所有权只能被一个线程拥有。
            通过互斥锁可以锁定一个代码块, 被锁定的这个代码块, 所有的线程只能顺序执行(不能并行处理)，这样多线程访问共享资源数据混乱的问题就可以被解决了，
            需要付出的代价就是执行效率的降低，因为默认临界区多个线程是可以并行处理的，现在只能串行处理。

        互斥锁数据类型 pthread_mutex_t。里面包含着两个信息
        pthread_mutex_t  mutex;
            1.锁的状态，锁着还是打开着
            2.增加锁的线程信息，因为只有增加所的线程，才能够给打开这个锁
        一般情况下，每一个共享资源都对应着一个互斥锁，锁的个数和线程个数无关
        // 初始化互斥锁
        // restrict: 是一个关键字, 用来修饰指针, 只有这个关键字修饰的指针可以访问指向的内存地址, 其他指针是不行的，即同一个地址的指针（指针别名之类的）不可以操作这个地址
        int pthread_mutex_init(pthread_mutex_t *restrict mutex,
                const pthread_mutexattr_t *restrict attr);
        
        // 释放互斥锁资源            
        int pthread_mutex_destroy(pthread_mutex_t *mutex);
        参数:
            mutex: 互斥锁变量的地址
            attr: 互斥锁的属性, 一般使用默认属性即可, 这个参数指定为NULL
        
        // 修改互斥锁的状态, 将其设定为锁定状态, 这个状态被写入到参数 mutex 中
        int pthread_mutex_lock(pthread_mutex_t *mutex);
        这个函数被调用, 首先会判断参数 mutex 互斥锁中的状态是不是锁定状态:
            1.没有被锁定, 是打开的, 这个线程可以加锁成功, 这个这个锁中会记录是哪个线程加锁成功了
            2.如果被锁定了, 其他线程加锁就失败了, 这些线程都会阻塞在这把锁上
            3.当这把锁被解开之后, 这些阻塞在锁上的线程就解除阻塞了，并且这些线程是通过竞争的方式对这把锁加锁，没抢到锁的线程继续阻塞
        
        // 尝试加锁
        int pthread_mutex_trylock(pthread_mutex_t *mutex);
        调用这个函数对互斥锁变量加锁还是有两种情况:
            1.如果这把锁没有被锁定是打开的，线程加锁成功
            2.如果锁变量被锁住了，调用这个函数加锁的线程，不会被阻塞，加锁失败直接返回错误号
        !trylock和lock函数的区别就在于，trylock尝试加锁失败后，不会被堵塞在这里，会先去执行自己的代码段，而不是一直等待开锁

        // 对互斥锁解锁
        int pthread_mutex_unlock(pthread_mutex_t *mutex);
        要注意：不是所有的线程都可以对互斥锁解锁，哪个线程加的锁, 哪个线程才能解锁成功。

        要注意，在创建互斥锁对象时，要保证线程同步期间，锁对应的资源是有效的。

    */


    return 0;
}

void * func_A(void * args){
    for(int i=0; i<MAX; ++i)
    { // 临界资源是number，和number相关的代码都是临界区
        pthread_mutex_lock(&mutex); // 加锁
        int cur = number;
        cur++;
        usleep(10);
        number = cur;
        printf("Thread A, id = %lu, number = %d\n", pthread_self(), number);
        pthread_mutex_unlock(&mutex); // 解锁
    }

    return NULL;

}
void * func_B(void * args){
    for(int i=0; i<MAX; ++i)
    {
        pthread_mutex_lock(&mutex); // 加锁
        int cur = number;
        cur++;
        number = cur;
        printf("Thread B, id = %lu, number = %d\n", pthread_self(), number);
        pthread_mutex_unlock(&mutex); // 解锁
        usleep(5);
    }

    return NULL;

}