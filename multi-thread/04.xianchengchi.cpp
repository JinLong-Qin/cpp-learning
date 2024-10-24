#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "04.ThreadPool.h"

using namespace std;


void taskFunction(void* arg){
    int num = *(int*) arg;
    printf("thread %ld is workding , number= %d. \n", pthread_self(), num);
    sleep(1);
}


int main(){
    // 线程池 cpp

    ThreadPool<int> pool(3, 10);

    for (int i = 0; i < 100; i++)
    {
        int* num = new int(i+100);
        pool.addTask(Task<int>(taskFunction, num));
    }
    
    sleep(20);


    return 0;
}