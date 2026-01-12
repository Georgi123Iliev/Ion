#pragma once
#include <vector>
#include<thread>

#include "SafeQueue.h"


using Task = std::function<void()>;

using TaskQueue = SafeQueue<Task>;


class ThreadPool
{

public:

	ThreadPool(unsigned int desiredWorkerCount);

	ThreadPool();

	~ThreadPool();

	[[nodiscard]]
	auto getWorkerCount() const noexcept
	{
		return m_workerCount;
	}

	void submit(Task task);


private:
	std::vector<std::jthread> m_workerThreads;
	
	TaskQueue m_taskQueue;



	unsigned int m_workerCount;

	void IntiateWork();


	


};

