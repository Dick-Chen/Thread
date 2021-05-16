// ʹ����������ʵ�������ߺ�������ģ�ͣ�
// �������� 5 ����������ͷ����ӽڵ㣬������Ҳ�� 5 ����ɾ������ͷ���Ľڵ㡣

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

//������
void* producer(void* arg)
{
    while(1)
    {
        //----------����-----------
        pthread_mutex_lock(&mutex);
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->number = rand() % 1000;
        newNode->next = head;
        head = newNode;
        printf("������ id = %ld, number = %d\n", pthread_self(), newNode->number);
        pthread_mutex_unlock(&mutex);
        //----------����-----------

        //������һ����֪ͨ�����߿�������,�������
        pthread_cond_broadcast(&cond);

        sleep(rand() % 3);
    }
    return NULL;
}

//������
void *consumer(void *arg)
{
    while(1)
    {
        //----------����-----------
        pthread_mutex_lock(&mutex);
        while(head==NULL)
        {
            //������ǰ������,ֱ���յ������ߵ�����
            pthread_cond_wait(&cond, &mutex);
        }

        struct Node* pnode = head;
        printf("������ id = %ld, number = %d\n", pthread_self(), pnode->number);
        head  = pnode->next;
        free(pnode);
        pthread_mutex_unlock(&mutex);
        //----------����-----------

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