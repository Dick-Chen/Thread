// 使用条件变量实现生产者和消费者模型，
// 生产者有 5 个，往链表头部添加节点，消费者也有 5 个，删除链表头部的节点。

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_cond_t cond;
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
        //----------上锁-----------
        pthread_mutex_lock(&mutex);
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->number = rand() % 1000;
        newNode->next = head;
        head = newNode;
        printf("生产者 id = %ld, number = %d\n", pthread_self(), newNode->number);
        pthread_mutex_unlock(&mutex);
        //----------解锁-----------

        //生产完一个，通知消费者可以消费,解除阻塞
        pthread_cond_broadcast(&cond);

        sleep(rand() % 3);
    }
    return NULL;
}

//消费者
void *consumer(void *arg)
{
    while(1)
    {
        //----------上锁-----------
        pthread_mutex_lock(&mutex);
        while(head==NULL)
        {
            //阻塞当前消费者,直到收到生产者的条件
            pthread_cond_wait(&cond, &mutex);
        }

        struct Node* pnode = head;
        printf("消费者 id = %ld, number = %d\n", pthread_self(), pnode->number);
        head  = pnode->next;
        free(pnode);
        pthread_mutex_unlock(&mutex);
        //----------解锁-----------

        sleep(rand() % 3);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

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
    pthread_cond_destroy(&cond);
    return 0;
}