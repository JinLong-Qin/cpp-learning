#include <stdio.h>
#include <pthread.h>

#define MAX 50
int number; // 记录当前数数的值

pthread_mutex_t mutex; // 创建互斥锁对象，全局变量保证其在运行期间不会被释放掉

//! 死锁
// 当多个线程访问共享资源, 需要加锁, 如果锁使用不当, 就会造成死锁这种现象。
// 如果线程死锁造成的后果是：所有的线程都被阻塞，并且线程的阻塞是无法解开的（因为可以解锁的线程也被阻塞了）。

// 1.加锁之后没有解锁
// 场景1
void func()
{
    for(int i=0; i<6; ++i)
    {
        // 当前线程A加锁成功, 当前循环完毕没有解锁, 在下一轮循环的时候自己被阻塞了
        // 其余的线程也被阻塞
    	pthread_mutex_lock(&mutex);
    	....
    	.....
        // 忘记解锁
    }
}

// 场景2
void func()
{
    for(int i=0; i<6; ++i)
    {
        // 当前线程A加锁成功
        // 其余的线程被阻塞
    	pthread_mutex_lock(&mutex);
    	....
    	.....
        if(xxx)
        {
            // 函数退出, 没有解锁（解锁函数无法被执行了）
            return ;
        }
        
        pthread_mutex_unlock(&mutex);
    }
}

// 2.重复加锁, 造成死锁
void func()
{
    for(int i=0; i<6; ++i)
    {
        // 当前线程A加锁成功
        // 其余的线程阻塞
    	pthread_mutex_lock(&mutex);
        // 锁被锁住了, A线程阻塞
        pthread_mutex_lock(&mutex);
    	....
    	.....
        pthread_mutex_unlock(&mutex);
    }
}

// 隐藏的比较深的情况
void funcA()
{
    for(int i=0; i<6; ++i)
    {
        // 当前线程A加锁成功
        // 其余的线程阻塞
    	pthread_mutex_lock(&mutex);
    	....
    	.....
        pthread_mutex_unlock(&mutex);
    }
}

void funcB()
{
    for(int i=0; i<6; ++i)
    {
        // 当前线程A加锁成功
        // 其余的线程阻塞
    	pthread_mutex_lock(&mutex);
        funcA();		// 重复加锁
    	....
    	.....
        pthread_mutex_unlock(&mutex);
    }
}

// 3.在程序中有多个共享资源, 因此有很多把锁，随意加锁，导致相互被阻塞
/*
场景描述:
  1. 有两个共享资源:X, Y，X对应锁A, Y对应锁B
     - 线程A访问资源X, 加锁A
     - 线程B访问资源Y, 加锁B
  2. 线程A要访问资源Y, 线程B要访问资源X，因为资源X和Y已经被对应的锁锁住了，因此这个两个线程被阻塞
     - 线程A被锁B阻塞了, 无法打开A锁
     - 线程B被锁A阻塞了, 无法打开B锁

  ！！！不要一下访问多个临界资源，否则就应该将多个临界资源视为一个


*/


/**
 * 在使用多线程编程的时候，如何避免死锁呢？
 *      1.避免多次锁定, 多检查
 *      2.对共享资源访问完毕之后, 一定要解锁，或者在加锁的使用 trylock
 *      3.如果程序中有多把锁, 可以控制对锁的访问顺序(顺序访问共享资源，但在有些情况下是做不到的)，另外也可以在对其他互斥锁做加锁操作之前，先释放当前线程拥有的互斥锁。
 *      4.项目程序中可以引入一些专门用于死锁检测的模块
 */


int main(){
    



    return 0;
}
