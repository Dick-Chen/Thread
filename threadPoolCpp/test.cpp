#include <iostream>
#include <unistd.h>
#include <string>
#include "ThreadPool.h"
#include "ThreadPool.cpp"

using namespace std;

void taskFunc(void* arg)
{
    int num = *(int*)(arg);
    cout << "thread " << to_string(pthread_self()) << " is working, number = " << num << endl;
    sleep(1);
}

int main()
{
    //创建线程池
    ThreadPool<int> pool(3, 10);

    //添加任务
    for (int i = 0; i < 100; ++i)
    {
        int* num = new int(i + 100);
        pool.addTask(Task<int>(taskFunc, num));
    }
    sleep(20);

    return 0;
}