// 线程池结构体
typedef struct ThreadPool ThreadPool;

// 创建线程池并初始化
ThreadPool* threadPool_create(int minNum, int maxNum, int queuesCapacity); // 最小线程数，最大线程数，任务队列容量（用于给任务队列分配空间

// 销毁线程池
int threadPoolDestory(ThreadPool* pool);

// 给线程池添加任务
void threadPoolAdd(ThreadPool* pool, void(*func) (void*), void* arg); // 线程池，task.function, task.arg

// 得到当前线程池正在工作的线程数
int threadPoolBusyNum(ThreadPool* pool);

// 得到线程池中活着的线程数
int threadPoolLiveNum(ThreadPool* pool);

///////////////////
// 工作线程
void* worker(void* arg);
//////////////////
// 管理线程
void* manager(void* arg);
// 目的是在线程退出的时候，将thradids中对应自己线程位置的值改为0，以便这片地址的重复利用
void threadExit(ThreadPool* pool);