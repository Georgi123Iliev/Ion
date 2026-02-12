#include "pch.h"
#include "ThreadPool.h"

namespace Ion::Threading
{


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
		//explicitly enforcing cleanup order 

		m_taskQueue.shutdown();

		m_workerThreads.clear();
	}



	void ThreadPool::dispatch(Task task)
	{
		m_taskQueue.push(std::move(task));
	}

	void ThreadPool::IntiateWork()
	{
		auto workerAction = [&taskQueue = m_taskQueue]() {

			//loop while the queue functions
			while (auto task = taskQueue.pop())
			{
				(*task)(); //calling the task
			}
			};

		for (size_t i = 0; i < m_workerCount; i++)
		{
			m_workerThreads.emplace_back(workerAction);
		}
	}

}