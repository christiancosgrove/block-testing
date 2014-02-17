#include "ThreadPool.h"


ThreadPool::ThreadPool(int _numThreads) : threads(_numThreads)
{
	for (int i = 0; i<_numThreads; i++)
	{
		threads[i] = std::thread(&ThreadPool::threadMain, this);
	}
}


ThreadPool::~ThreadPool()
{
}

void ThreadPool::NewAction(std::function<void()> action)
{
	actions.push_back(action);
	actionsConditionVariable.notify_one();
}

void ThreadPool::threadMain()
{
	std::function<void()> nextAction;
	{
		std::unique_lock<std::mutex> lk(actionsMutex);
		actionsConditionVariable.wait(lk, [this]{ return actions.size() > 0; });
		nextAction = actions.front();
		actions.pop_front();
	}
	nextAction();
	threadMain();
}