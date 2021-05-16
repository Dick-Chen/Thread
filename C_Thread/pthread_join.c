/* 三、线程回收函数
这是一个阻塞函数, 子线程在运行这个函数就阻塞
子线程退出, 函数解除阻塞, 回收对应的子线程资源, 类似于回收进程使用的函数 wait()
int pthread_join(pthread_t thread, void **retval);
参数:
    thread: 要被回收的子线程的线程 ID
    retval: 二级指针，指向一级指针的地址，是一个传出参数，
            这个地址中存储了 pthread_exit () 传递出的数据，如果不需要这个参数，可以指定为 NULL
返回值：
    线程回收成功返回 0，回收失败返回错误号。 */

/* 四、线程分离函数
int pthread_detach(pthread_t thread);
参数就子线程的线程ID, 主线程就可以和这个子线程分离了

在某些情况下，程序中的主线程有属于自己的业务处理流程，
如果让主线程负责子线程的资源回收，调用 pthread_join() 
只要子线程不退出主线程就会一直被阻塞，
主要线程的任务也就不能被执行了。

调用线程分离函数之后指定的子线程就可以和主线程分离，
当子线程退出的时候，其占用的内核资源就被系统的其他进程接管并回收了。
线程分离之后在主线程中使用 pthread_join() 就回收不到子线程资源了。 */

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
    pthread_create(&tid, NULL, working, &t);
    printf("主线程id: %ld\n", pthread_self());

    // 设置子线程和主线程分离
    pthread_detach(tid);

    //线程回收
    // void* ptr = NULL;
    // pthread_join(tid, &ptr);
    // struct Test *pt = (struct Test *)ptr;
    // printf("num: %d, age: %d\n", pt->num, pt->age);
    // printf("num: %d, age: %d\n", t.num, t.age);


    pthread_exit(NULL);

    return 0;
}