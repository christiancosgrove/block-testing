#pragma once
#include <vector>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>


class ThreadPool
{
public:
	ThreadPool(int _numThreads);
	~ThreadPool();
	void NewAction(std::function<void()> action);
private:
	void threadMain();
	std::vector<std::thread> threads;
	std::deque<std::function<void()>> actions;
	std::mutex actionsMutex;
	std::condition_variable actionsConditionVariable;
};

