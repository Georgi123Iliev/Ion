#pragma once
#include<queue>
#include<optional>
#include<mutex>
#include<condition_variable>
#include <concepts>
#include<functional>
#include <stdexcept>
#include <utility>



template<typename T>
requires std::movable<T>
class SafeQueue
{
public:

	
	
	void push(const T& t) requires std::copyable<T>
	{
		{
			const std::lock_guard guard(m_mutex);

			if (stop)
			{
				throw std::runtime_error("Queue is stopped");
			}

			m_queue.push(t);
		}

		m_conditionVariable.notify_one();
	}

	void push(T&& t) {

		{
			const std::lock_guard guard(m_mutex);

			if (stop)
			{
				throw std::runtime_error("Queue is stopped");
			}


			m_queue.push(std::move(t));
		}

		m_conditionVariable.notify_one();

	}


	[[nodiscard]] std::optional<T> pop()
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		//wait until the queue is not empty OR we have stopped the queue
		//  !m_queue.empty() means no data to use
		//  stop means we are done with data
		m_conditionVariable.wait(lock, [this] { return !m_queue.empty() || stop; });

		if (m_queue.empty())
		{
			return std::nullopt;
		}
		else
		{
			//construct a std::optional with the value to be returned
			auto optVal = std::make_optional<T>(std::move(m_queue.front()));

			m_queue.pop();

			return optVal;

		}


	}

	void shutdown()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		stop = true;
		lock.unlock();
		m_conditionVariable.notify_all();
	}

private:



	std::mutex m_mutex;
	std::condition_variable m_conditionVariable;

	bool stop = false;
	std::queue<T> m_queue;

	

};

