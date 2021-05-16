/* 使用信号量实现生产者和消费者模型，
生产者有 5 个，往链表头部添加节点，消费者也有 5 个，删除链表头部的节点。 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

//生产者的信号量
sem_t semp;
//消费者的信号量
sem_t semc;

pthread_mutex_t mutex;

struct Node{
    int number;
    struct Node *next;
};

struct Node *head = NULL;

//生产者
void* producer(void* arg)
{
    while(1)
    {
        //生产者等待资源，没有则阻塞
        sem_wait(&semp);

        //----------上锁-----------
        pthread_mutex_lock(&mutex);
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->number = rand() % 1000;
        newNode->next = head;
        head = newNode;
        printf("生产者 id = %ld, number = %d\n", pthread_self(), newNode->number);        
        pthread_mutex_unlock(&mutex);
        //----------解锁-----------

        //生产者告诉消费者有可用资源
        sem_post(&semc);

        sleep(rand() % 3);
    }
    return NULL;
}

//消费者
void *consumer(void *arg)
{
    while(1)
    {
        //消费者等待资源，没有则阻塞
        sem_wait(&semc);

        //----------上锁-----------
        pthread_mutex_lock(&mutex);
        struct Node *node = head;
        printf("消费者 id = %ld, number = %d\n", pthread_self(), node->number);
        head = node->next;
        free(node);     
        pthread_mutex_unlock(&mutex);
        //----------解锁-----------

        //消费者告诉生产者有可用的资源
        sem_post(&semp);

        sleep(rand() % 3);       
    }
    return NULL;
}

int main()
{
    //生产者
    sem_init(&semp, 0, 5);
    //消费者->资源初始化为0
    sem_init(&semc, 0, 0);

    pthread_mutex_init(&mutex, NULL);

    pthread_t t1[5], t2[5];

    for (int i = 0; i < 5;++i){
        pthread_create(&t1[i], NULL, producer, NULL);
    }

    for (int i = 0; i < 5;++i){
        pthread_create(&t2[i], NULL, consumer, NULL);
    }

    for (int i = 0; i < 5;++i){
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    }


    pthread_mutex_destroy(&mutex);
    sem_destroy(&semp);
    sem_destroy(&semc);

    return 0;
}