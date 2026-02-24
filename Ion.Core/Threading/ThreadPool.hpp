#pragma once
#include <vector>
#include <thread>
#include <concepts>
#include <future>
#include <functional>
#include<memory>

#include "SafeQueue.hpp"

namespace Ion::Threading
{

	using Task = std::move_only_function<void()>;

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

			auto pkgTask = std::packaged_task<returnType()>(std::move(callable));

			auto futureResult = pkgTask.get_future();

			auto wrapper = [Task = std::move(pkgTask)]() mutable { Task(); };

			dispatch(std::move(wrapper));

			return futureResult;
		}
		



	private:
		std::vector<std::jthread> m_workerThreads;

		TaskQueue m_taskQueue;



		unsigned int m_workerCount;

		void InitiateWork();






	};

}