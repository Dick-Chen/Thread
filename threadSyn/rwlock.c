//8 个线程操作同一个全局变量，3 个线程不定时写同一全局资源，5 个线程不定时读同一全局资源
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define N 50

int num;

pthread_rwlock_t rwlock;

// 线程处理函数
void *read_num(void* arg)
{
    for (int i = 0; i < N;i++)
    {
        //----------加读锁-----------
        pthread_rwlock_rdlock(&rwlock);
        printf("thread read, id = %lu, number = %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        //----------解读锁-----------
        usleep(rand() % 5);
    }
    return NULL;
}

void *write_num(void* arg)
{
    for (int i = 0; i < N;i++)
    {
        //----------加写锁-----------
        pthread_rwlock_wrlock(&rwlock);
        int cur = num;
        cur++;
        num = cur;
        printf("thread write, id = %lu, number = %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        //----------解锁-----------
        usleep(rand() % 5);
    }
    return NULL;
}

int main()
{
    pthread_t rp[5], wp[3];
    pthread_rwlock_init(&rwlock, NULL);

    //读子线程
    for (int i = 0; i < 5;++i){
        pthread_create(&rp[i], NULL, read_num, NULL);   
    }

    //写子线程
    for (int i = 0; i < 3;++i){
        pthread_create(&wp[i], NULL, write_num, NULL);
    }

    for (int i = 0; i < 5;++i){
        pthread_join(rp[i], NULL);
    }

    for (int i = 0; i < 3;++i){
        pthread_join(wp[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);

    return 0;
}