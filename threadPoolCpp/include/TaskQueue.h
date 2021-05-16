#pragma once
#include <queue>
#include <pthread.h>

using callback = void (*)(void*);
//任务结构体
template <typename T>
struct Task
{
	Task()
	{
		function = nullptr;
		arg = nullptr;
	}

	Task(callback f,void *arg)
	{
		function = f;
		this->arg = (T*)arg;
	}

	callback function;
	T* arg;
};

template <typename T>
class TaskQueue
{
public:
	TaskQueue();
	~TaskQueue();

	//添加一个任务
	void addTask(Task<T>& task);
	void addTask(callback f, void* arg);
	//取出一个任务
	Task<T> takeTask();
	//获取当前任务的个数
	inline size_t taskNumber()
	{
		return m_taskQ.size();
	}

private:
	pthread_mutex_t m_mutex;
	std::queue<Task<T>> m_taskQ;
};

