#pragma once
#include <vector>
#include <thread>
#include <concepts>
#include <future>
#include<memory>

#include "SafeQueue.h"

namespace Ion::Threading
{

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

		void dispatch(Task task);

		template<typename Callable>
		requires std::invocable<Callable>
		[[nodiscard("Discarding std::future returned. This method should be used only when you need a result/confirmation task has finished")]]
		std::future<std::invoke_result_t<Callable>> submit(Callable callable)
		{

			using returnType = std::invoke_result_t<Callable>;

			auto pkgTaskSharedPtr = std::make_shared<std::packaged_task<returnType()>>(std::move(callable));

			auto futureResult = pkgTaskSharedPtr->get_future();

			auto wrapper = [pkgTaskSharedPtr]() { (*pkgTaskSharedPtr)(); };

			dispatch(wrapper);

			return futureResult;
		}
		



	private:
		std::vector<std::jthread> m_workerThreads;

		TaskQueue m_taskQueue;



		unsigned int m_workerCount;

		void IntiateWork();






	};

}