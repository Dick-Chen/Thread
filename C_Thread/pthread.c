
/*一、线程创建函数 
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);
参数:
    thread: 传出参数，是无符号长整形数，线程创建成功，会将线程 ID 写入到这个指针指向的内存中
    attr: 线程的属性，一般情况下使用默认属性即可，写 NULL
    start_routine: 函数指针，创建出的子线程的处理动作，也就是该函数在子线程中执行。
    arg: 作为实参传递到 start_routine 指针指向的函数内部
返回值：
    线程创建成功返回 0，创建失败返回对应的错误号 */

/* 二、线程退出函数
void pthread_exit(void *retval);
参数：
    线程退出的时候携带的数据，当前子线程的主线程会得到该数据。如果不需要使用，指定为 NULL */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
//编译要链接库libpthread.so libptread.a

struct Test
{
    int num;
    int age;
};

void *working(void *arg)
{
    for (int i = 0; i < 5;i++)
    {
        printf("子线程:i=%d\n", i);
    }
    printf("子线程id:%ld\n", pthread_self());

    struct Test *t = (struct Test *)arg;
    t->num = 100;
    t->age = 6;

    //2、线程退出，不会导致虚拟地址空间的释放，即不会影响到其他线程的正常运行
    pthread_exit(t);
    return NULL;
}

int main()
{
    struct Test t;
    pthread_t tid;

    //1、创建线程
    pthread_create(&tid, NULL, working, &t);
    printf("子线程创建成功, 线程ID: %ld\n", tid);

    sleep(1);//主线程阻塞1s
    for (int i = 0; i < 5;i++){
        printf("主线程：i=%d\n", i);
    }
    printf("主线程id: %ld\n", pthread_self());
    printf("Test t %d %d\n", t.age, t.num);

    pthread_exit(NULL);

    return 0;
}
