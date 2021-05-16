/* �����̻߳��պ���
����һ����������, ���߳��������������������
���߳��˳�, �����������, ���ն�Ӧ�����߳���Դ, �����ڻ��ս���ʹ�õĺ��� wait()
int pthread_join(pthread_t thread, void **retval);
����:
    thread: Ҫ�����յ����̵߳��߳� ID
    retval: ����ָ�룬ָ��һ��ָ��ĵ�ַ����һ������������
            �����ַ�д洢�� pthread_exit () ���ݳ������ݣ��������Ҫ�������������ָ��Ϊ NULL
����ֵ��
    �̻߳��ճɹ����� 0������ʧ�ܷ��ش���š� */

/* �ġ��̷߳��뺯��
int pthread_detach(pthread_t thread);
���������̵߳��߳�ID, ���߳̾Ϳ��Ժ�������̷߳�����

��ĳЩ����£������е����߳��������Լ���ҵ�������̣�
��������̸߳������̵߳���Դ���գ����� pthread_join() 
ֻҪ���̲߳��˳����߳̾ͻ�һֱ��������
��Ҫ�̵߳�����Ҳ�Ͳ��ܱ�ִ���ˡ�

�����̷߳��뺯��֮��ָ�������߳̾Ϳ��Ժ����̷߳��룬
�����߳��˳���ʱ����ռ�õ��ں���Դ�ͱ�ϵͳ���������̽ӹܲ������ˡ�
�̷߳���֮�������߳���ʹ�� pthread_join() �ͻ��ղ������߳���Դ�ˡ� */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

struct Test
{
    int num;
    int age;
};

void *working(void *arg)
{
    for (int i = 0; i < 5;i++)
    {
        printf("���߳�:i=%d\n", i);
    }
    printf("���߳�id:%ld\n", pthread_self());

    struct Test *t = (struct Test *)arg;
    t->num = 100;
    t->age = 6;

    //2���߳��˳������ᵼ�������ַ�ռ���ͷţ�������Ӱ�쵽�����̵߳���������
    pthread_exit(t);
    return NULL;
}


int main()
{
    struct Test t;
    pthread_t tid;
    pthread_create(&tid, NULL, working, &t);
    printf("���߳�id: %ld\n", pthread_self());

    // �������̺߳����̷߳���
    pthread_detach(tid);

    //�̻߳���
    // void* ptr = NULL;
    // pthread_join(tid, &ptr);
    // struct Test *pt = (struct Test *)ptr;
    // printf("num: %d, age: %d\n", pt->num, pt->age);
    // printf("num: %d, age: %d\n", t.num, t.age);


    pthread_exit(NULL);

    return 0;
}