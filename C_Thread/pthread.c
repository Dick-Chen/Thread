
/*һ���̴߳������� 
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);
����:
    thread: �������������޷��ų����������̴߳����ɹ����Ὣ�߳� ID д�뵽���ָ��ָ����ڴ���
    attr: �̵߳����ԣ�һ�������ʹ��Ĭ�����Լ��ɣ�д NULL
    start_routine: ����ָ�룬�����������̵߳Ĵ�������Ҳ���Ǹú��������߳���ִ�С�
    arg: ��Ϊʵ�δ��ݵ� start_routine ָ��ָ��ĺ����ڲ�
����ֵ��
    �̴߳����ɹ����� 0������ʧ�ܷ��ض�Ӧ�Ĵ���� */

/* �����߳��˳�����
void pthread_exit(void *retval);
������
    �߳��˳���ʱ��Я�������ݣ���ǰ���̵߳����̻߳�õ������ݡ��������Ҫʹ�ã�ָ��Ϊ NULL */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
//����Ҫ���ӿ�libpthread.so libptread.a

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

    //1�������߳�
    pthread_create(&tid, NULL, working, &t);
    printf("���̴߳����ɹ�, �߳�ID: %ld\n", tid);

    sleep(1);//���߳�����1s
    for (int i = 0; i < 5;i++){
        printf("���̣߳�i=%d\n", i);
    }
    printf("���߳�id: %ld\n", pthread_self());
    printf("Test t %d %d\n", t.age, t.num);

    pthread_exit(NULL);

    return 0;
}
