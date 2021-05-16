/* 使用互斥锁实现两个线程交替数数（每个线程数 50 个数，交替数到 100） */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define N  50

int num;

pthread_mutex_t mutex;

// 线程处理函数
void *funA(void* arg)
{
    for (int i = 0; i < N; i++)
    {
        //----------上锁-----------
        pthread_mutex_lock(&mutex);
        int cur = num;
        cur++;
        usleep(10);
        num = cur;
        printf("thread A, id = %lu, number = %d\n", pthread_self(), num);
        pthread_mutex_unlock(&mutex);
        //----------解锁-----------
    }

    return NULL;
}

void *funB(void* arg)
{
    for (int i = 0; i < N; i++)
    {
        //----------上锁-----------
        pthread_mutex_lock(&mutex);
        int cur = num;
        cur++;
        num = cur;
        printf("thread B, id = %lu, number = %d\n", pthread_self(), num);
        pthread_mutex_unlock(&mutex);
        //----------解锁-----------

        usleep(10);
    }

    return NULL;
}

int main()
{
    pthread_t p1, p2;

    //1、初始化互斥锁
    pthread_mutex_init(&mutex, NULL);

    //2、创建两个子线程
    pthread_create(&p1, NULL, funA, NULL);
    pthread_create(&p2, NULL, funB, NULL);

    //3、阻塞回收
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    //4、销毁互斥锁
    pthread_mutex_destroy(&mutex);

    return 0;
}