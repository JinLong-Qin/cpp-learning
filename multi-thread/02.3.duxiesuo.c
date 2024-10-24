#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For usleep()

#define MAX 50
// 全局资源
int number;

pthread_rwlock_t rwlock; // 读写锁

// 写的线程的处理函数
void* writeNum(void* arg)
{
    int i = 0;
    while(i<MAX)
    {
        pthread_rwlock_wrlock(&rwlock); // 加写锁
        int cur = number;
        cur ++;
        number = cur;
        printf("++写操作完毕, tid : %ld, number = %d\n", pthread_self(), number );
        pthread_rwlock_unlock(&rwlock); // 解锁
        // 添加sleep目的是要看到多个线程交替工作
        usleep(rand() % 100);
        ++i;
    }

    return NULL;
}

// 读线程的处理函数
// 多个线程可以如果处理动作相同, 可以使用相同的处理函数
// 每个线程中的栈资源是独享
void* readNum(void* arg)
{
    int i = 0;
    while(i<MAX)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("--全局变量, tid = %ld, number = %d\n", pthread_self(), number);
        pthread_rwlock_unlock(&rwlock);
        usleep(rand() % 100);
        ++i;
    }
    return NULL;
}


int main(){
    // 读写锁，不是两把锁，只是一把锁
    // 读写锁是互斥锁的升级版, 在做读操作的时候可以提高程序的执行效率，如果所有的线程都是做读操作, 那么读是并行的，但是使用互斥锁，读操作也是串行的。
    /*
    读写锁是一把锁，锁的类型为pthread_rwlock_t，有了类型之后就可以创建一把互斥锁了：
    pthread_rwlock_t rwlock;
    之所以称其为读写锁，是因为这把锁既可以锁定读操作，也可以锁定写操作。读写锁中记录的信息包括：
        1. 锁定状态，锁定或者打开
        2. 锁定的是什么状态，读或者写。 使用读写锁锁定了读操作，需要先解锁才能去锁定写操作，反之亦然。
        3. 哪一个线程把这个锁锁上了

    读写锁的使用方式也互斥锁的使用方式是完全相同的：找共享资源, 确定临界区，在临界区的开始位置加锁（读锁/写锁），临界区的结束位置解锁。

    读写锁的特点： 在同一时间内
        1.使用读写锁的读锁锁定了临界区，线程对临界区的访问是并行的，[读锁是共享的。]
        2.使用读写锁的写锁锁定了临界区，线程对临界区的访问是串行的，[写锁是独占的。]
        3.使用读写锁分别对两个临界区加了读锁和写锁，两个线程要同时访问者两个临界区，访问写锁临界区的线程继续运行，访问读锁临界区的线程阻塞，因为[写锁比读锁的优先级高]。

    注意：如果说程序中所有的线程都对共享资源做写操作，使用读写锁没有优势，和互斥锁是一样的，
          如果说程序中所有的线程都对共享资源有写也有读操作，并且对共享资源读的操作越多，读写锁更有优势。

    // 要保证锁对应的共享资源一定是有效的，不会被释放或者析构

    // 初始化读写锁 restrict关键字修饰的指针，表明其他任意指向这个地址的指针都无法操作这片内存
    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
            const pthread_rwlockattr_t *restrict attr);
    // 释放读写锁占用的系统资源
    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
    参数:
        rwlock: 读写锁的地址，传出参数
        attr: 读写锁属性，一般使用默认属性，指定为NULL

    // 在程序中对读写锁加读锁, 锁定的是读操作  rd指read
    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
    调用这个函数，如果读写锁是打开的，那么加锁成功；
    如果读写锁已经锁定了读操作，调用这个函数依然可以加锁成功，因为读锁是共享的；
    如果读写锁已经锁定了写操作，调用这个函数的线程会被阻塞。

    // 这个函数可以有效的避免死锁
    // 如果加读锁失败, 不会阻塞当前线程, 直接返回错误号
    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
    调用这个函数，如果读写锁是打开的，那么加锁成功；
    如果读写锁已经锁定了读操作，调用这个函数依然可以加锁成功，因为读锁是共享的；
    如果读写锁已经锁定了写操作，调用这个函数加锁失败，对应的线程不会被阻塞，可以在程序中对函数返回值进行判断，添加加锁失败之后的处理动作。

    // 在程序中对读写锁加写锁, 锁定的是写操作 wr指write
    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
    调用这个函数，如果读写锁是打开的，那么加锁成功；
    如果读写锁已经锁定了读操作或者锁定了写操作，调用这个函数的线程会被阻塞。

    // 这个函数可以有效的避免死锁
    // 如果加写锁失败, 不会阻塞当前线程, 直接返回错误号
    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
    调用这个函数，如果读写锁是打开的，那么加锁成功；
    如果读写锁已经锁定了读操作或者锁定了写操作，调用这个函数加锁失败，但是线程不会阻塞，可以在程序中对函数返回值进行判断，添加加锁失败之后的处理动作。

    // 解锁, 不管锁定了读还是写都可用解锁
    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);


    一个资源只能被添加一个读锁或者写锁，
        如果增加了读锁，再增加读锁是可以的，即一个进程在读数据，也允许别的进程来读数据
        如果增加了写锁，则读锁和写锁都不可以再加，即一个进程在写数据，其他操作都要等待这个进程写完才行
    */
    
    // 示例， 创建8个线程同时操作一个全局变量，3个线程不定时写同一个全局资源，5个线程不定时读同一个全局资源
    // 读的比较多，就考虑使用读写锁，读的如果少，就基本和互斥锁差不多了

    // 初始化
    pthread_rwlock_init(&rwlock, NULL); // 初始化，锁的地址和NULL的锁类型

    // 3个写进程，5个都进程
    pthread_t wtid[3];
    pthread_t rtid[5];

    for (int i = 0; i < 3; ++i)
    {
        pthread_create(&wtid[i], NULL, writeNum, NULL); // 创建写进程，进程id，进程类型，进程操作函数，传递给进程操作函数的参数
    }

    for (int i = 0; i < 5; ++i)
    {
        pthread_create(&rtid[i], NULL, readNum, NULL);
    }

    // 释放资源
    for (int i = 0; i < 3; ++i)
    {
        pthread_join(&wtid[i], NULL); // 线程id，接收返回数据
    }

    for (int i = 0; i < 5; ++i)
    {
        pthread_join(&rtid[i], NULL); 
    }

    // 销毁读写锁
    pthread_rwlock_destroy(&rwlock);

    
    
    return 0;
}