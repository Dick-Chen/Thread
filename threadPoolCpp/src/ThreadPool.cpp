#include "ThreadPool.h"
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>

using namespace std;

template <typename T>
ThreadPool<T>::ThreadPool(int min, int max)
{
	//实例化任务队列
	taskQ = new TaskQueue<T>;
	do
	{
		threadIDs = new pthread_t[max];
		if (threadIDs == nullptr)
		{
			cout << "线程内存分配失败..." << endl;
			break;
		}
		//初始化
		memset(threadIDs, 0, sizeof(pthread_t) * max);
		minNum = min;
		maxNum = max;
		busyNum = 0;
		liveNum = min;	//和最小个数相等
		exitNum = 0;
		if (pthread_mutex_init(&mutexPool, NULL) != 0 ||
			pthread_cond_init(&notEmpty, NULL) != 0)
		{
			cout<<"互斥或条件变量初始化失败...\n"<<endl;
			break;
		}

		//创建线程
		pthread_create(&managerID, NULL, manager, this);
		for (int i = 0; i < min; ++i)
		{
			pthread_create(&threadIDs[i], NULL, worker, this);
		}
		return;

	} while (0);

	//释放资源
	if (threadIDs) delete []threadIDs;
	if (taskQ) delete taskQ;
}

template <typename T>
ThreadPool<T>::~ThreadPool()
{
	//关闭线程池
	shutdown = true;
	//阻塞回收管理者线程
	pthread_join(managerID, NULL);
	//唤醒阻塞的消费者线程
	for (int i = 0; i < liveNum; ++i)
	{
		pthread_cond_signal(&notEmpty);
	}
	//释放堆内存
	if (threadIDs)
	{
		delete []threadIDs;
	}
	if (taskQ)
	{
		delete taskQ;
	}
	pthread_mutex_destroy(&mutexPool);
	pthread_cond_destroy(&notEmpty);

}

template <typename T>
void ThreadPool<T>::addTask(Task<T> task)
{
	if (shutdown)
	{
		return;
	}

	//添加任务
	taskQ->addTask(task);

	//唤醒工作线程
	pthread_cond_signal(&notEmpty);

}

template <typename T>
int ThreadPool<T>::getBusyNum()
{
	pthread_mutex_lock(&mutexPool);
	int bNum = busyNum;
	pthread_mutex_unlock(&mutexPool);
	return bNum;
}

template <typename T>
int ThreadPool<T>::getAliveNum()
{
	pthread_mutex_lock(&mutexPool);
	int aliveNum = liveNum;
	pthread_mutex_unlock(&mutexPool);
	return aliveNum;
}

template <typename T>
void* ThreadPool<T>::worker(void* arg)
{
	ThreadPool * pool = static_cast<ThreadPool *>(arg);

	while (true)
	{
		pthread_mutex_lock(&pool->mutexPool);
		//当前任务是否为空
		while (pool->taskQ->taskNumber() == 0 && !pool->shutdown) {
			//阻塞工作线程,条件是直到不为空，加锁
			pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

			//判断是否要销毁线程，线程自杀
			if (pool->exitNum > 0)
			{
				pool->exitNum--;
				if (pool->liveNum > pool->minNum)
				{
					pool->liveNum--;
					//解锁
					pthread_mutex_unlock(&pool->mutexPool);
					pool->threadExit();
				}
			}
		}

		//判断线程池是否被关闭
		if (pool->shutdown) {
			pthread_mutex_unlock(&pool->mutexPool);
			pool->threadExit();
		}
		//从任务队列中取出一个任务
		Task<T> task = pool->taskQ->takeTask();

		pool->busyNum++;

		//解锁
		pthread_mutex_unlock(&pool->mutexPool);

		cout << "thread " << to_string(pthread_self()) << " start working..." << endl;
		task.function(task.arg);
		//(*task.function)(task.arg);
		delete task.arg;
		task.arg = nullptr;
		cout << "thread " << to_string(pthread_self()) << " end working..." << endl;

		pthread_mutex_lock(&pool->mutexPool);
		pool->busyNum--;
		pthread_mutex_unlock(&pool->mutexPool);
	}

	return NULL;
}

template <typename T>
void* ThreadPool<T>::manager(void* arg)
{
	ThreadPool * pool = static_cast<ThreadPool *>(arg);
	while (!pool->shutdown)
	{
		//每隔3秒检测一次
		sleep(5);

		//取出线程池中任务的数量,当前线程的数量,忙的线程的数量
		pthread_mutex_lock(&pool->mutexPool);
		int queueSize = pool->taskQ->taskNumber();
		int liveNum = pool->liveNum; 
		int busyNum = pool->busyNum;
		pthread_mutex_unlock(&pool->mutexPool);

		//添加线程
		//任务的个数>存活的线程数 && 存活的线程数<最大线程数
		if (queueSize > liveNum && liveNum < pool->maxNum)
		{
			pthread_mutex_lock(&pool->mutexPool);
			int counter = 0;
			for (int i = 0; i < pool->maxNum && counter < N && pool->liveNum < pool->maxNum; ++i)
			{
				if (pool->threadIDs[i] == 0)
				{
					pthread_create(&pool->threadIDs[i], NULL, worker, pool);
					counter++;
					pool->liveNum++;
				}
			}
			pthread_mutex_unlock(&pool->mutexPool);
		}
		//销毁线程
		//忙的线程*2<存活的线程树 && 存活的线程数>最小线程数
		if (busyNum * 2 < liveNum && liveNum > pool->minNum)
		{
			pthread_mutex_lock(&pool->mutexPool);
			pool->exitNum = N;
			pthread_mutex_unlock(&pool->mutexPool);
			//让工作线程自杀
			for (int i = 0; i < N; ++i)
			{
				pthread_cond_signal(&pool->notEmpty);
				//pthread_cond_broadcast(&pool->notEmpty);
			}
		}
	}
	return NULL;
}

template <typename T>
void ThreadPool<T>::threadExit()
{
	pthread_t tid = pthread_self();

	for (int i = 0; i < maxNum; ++i)
	{
		if (threadIDs[i] == tid)
		{
			threadIDs[i] = 0;
			cout<<"threadExit() called, "<<to_string(tid)<< " exiting..."<<endl;
			break;
		}
	}
	pthread_exit(NULL);
}

