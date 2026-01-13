#include "pch.h"
#include "ThreadPool.h"

 ThreadPool::ThreadPool(unsigned int desiredWorkerCount)
	: m_workerCount(desiredWorkerCount)
{
	m_workerThreads.reserve(m_workerCount);
	IntiateWork();
}

 ThreadPool::ThreadPool()
{
	const unsigned int cores = std::thread::hardware_concurrency();

	m_workerCount = (cores > 1) ? (cores - 1) : 1;

	m_workerThreads.reserve(m_workerCount);
	IntiateWork();

}

 ThreadPool::~ThreadPool()
{
	m_taskQueue.shutdown();

	for (auto& worker : m_workerThreads)
	{
		if (worker.joinable())
			worker.join();
	}
}



 void ThreadPool::submit(Task task)
{
	m_taskQueue.push(std::move(task));
}

 void ThreadPool::IntiateWork()
{
	auto workerAction = [this]() {

		//loop while the queue functions
		while (auto task = m_taskQueue.pop())
		{
			(*task)(); //calling the task
		}
		};

	for (int i = 0; i < m_workerCount; i++)
	{
		m_workerThreads.emplace_back(workerAction);
	}
}
