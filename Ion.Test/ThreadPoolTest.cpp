#include "pch.h"

#include "ThreadPool.h"


#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>


TEST(ThreadPoolTest, BasicFunctionality)
{
	std::atomic<int> counter = 0;

	{
		ThreadPool threadPool;

		

		auto increment = [&counter]() {counter++; };

		for (int i = 0; i < 1000; i++)
		{
			threadPool.submit(increment);
		}
	}

	ASSERT_EQ(counter, 1000);
}

TEST(ThreadPoolTest, TrueParallelism)
{
	using namespace std::chrono_literals;
	using namespace std::chrono;

	auto timeConsumingTask = []() { std::this_thread::sleep_for(1s); };

	{
		ThreadPool threadPool;

		auto before = high_resolution_clock::now();

		for (int i = 0; i < 30; i++)
		{
			threadPool.submit(timeConsumingTask);
		}

		auto after = high_resolution_clock::now();

		auto elapsed = after - before;

		ASSERT_LT(elapsed, 30s);

	}
}