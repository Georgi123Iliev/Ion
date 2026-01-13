#pragma once
#include <vector>
#include <thread>
#include <concepts>
#include <future>
#include<memory>

#include "SafeQueue.h"


using Task = std::function<void()>;

using TaskQueue = SafeQueue<Task>;

template<typename Callable>
concept NonVoidCallable =
std::invocable<Callable> && !std::is_same<std::invoke_result_t<Callable>, void>::value;




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

	template<NonVoidCallable Callable>
	[[nodiscard]] std::future<std::invoke_result_t<Callable>> submit(Callable callable) 
	{

		using returnType = std::invoke_result_t<Callable>;
		
		auto pkgTaskSharedPtr = std::make_shared<std::packaged_task<returnType()>>(std::move(callable));		
		
		auto futureResult = pkgTaskSharedPtr->get_future();

		auto wrapper = [pkgTaskSharedPtr]() { (*pkgTaskSharedPtr)(); };

		submit(wrapper);

		return futureResult;
	}

	


private:
	std::vector<std::jthread> m_workerThreads;
	
	TaskQueue m_taskQueue;



	unsigned int m_workerCount;

	void IntiateWork();


	


};

