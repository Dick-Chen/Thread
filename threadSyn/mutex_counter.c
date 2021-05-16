/* ʹ�û�����ʵ�������߳̽���������ÿ���߳��� 50 �������������� 100�� */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define N  50

int num;

pthread_mutex_t mutex;

// �̴߳�����
void *funA(void* arg)
{
    for (int i = 0; i < N; i++)
    {
        //----------����-----------
        pthread_mutex_lock(&mutex);
        int cur = num;
        cur++;
        usleep(10);
        num = cur;
        printf("thread A, id = %lu, number = %d\n", pthread_self(), num);
        pthread_mutex_unlock(&mutex);
        //----------����-----------
    }

    return NULL;
}

void *funB(void* arg)
{
    for (int i = 0; i < N; i++)
    {
        //----------����-----------
        pthread_mutex_lock(&mutex);
        int cur = num;
        cur++;
        num = cur;
        printf("thread B, id = %lu, number = %d\n", pthread_self(), num);
        pthread_mutex_unlock(&mutex);
        //----------����-----------

        usleep(10);
    }

    return NULL;
}

int main()
{
    pthread_t p1, p2;

    //1����ʼ��������
    pthread_mutex_init(&mutex, NULL);

    //2�������������߳�
    pthread_create(&p1, NULL, funA, NULL);
    pthread_create(&p2, NULL, funB, NULL);

    //3����������
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    //4�����ٻ�����
    pthread_mutex_destroy(&mutex);

    return 0;
}