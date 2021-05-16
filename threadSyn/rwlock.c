//8 ���̲߳���ͬһ��ȫ�ֱ�����3 ���̲߳���ʱдͬһȫ����Դ��5 ���̲߳���ʱ��ͬһȫ����Դ
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define N 50

int num;

pthread_rwlock_t rwlock;

// �̴߳�����
void *read_num(void* arg)
{
    for (int i = 0; i < N;i++)
    {
        //----------�Ӷ���-----------
        pthread_rwlock_rdlock(&rwlock);
        printf("thread read, id = %lu, number = %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        //----------�����-----------
        usleep(rand() % 5);
    }
    return NULL;
}

void *write_num(void* arg)
{
    for (int i = 0; i < N;i++)
    {
        //----------��д��-----------
        pthread_rwlock_wrlock(&rwlock);
        int cur = num;
        cur++;
        num = cur;
        printf("thread write, id = %lu, number = %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        //----------����-----------
        usleep(rand() % 5);
    }
    return NULL;
}

int main()
{
    pthread_t rp[5], wp[3];
    pthread_rwlock_init(&rwlock, NULL);

    //�����߳�
    for (int i = 0; i < 5;++i){
        pthread_create(&rp[i], NULL, read_num, NULL);   
    }

    //д���߳�
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