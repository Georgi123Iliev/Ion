#include "pch.h"

#include "ThreadPool.h"


#include <atomic>


TEST(ThreadPoolTest, BasicFunctionality)
{
	ThreadPool threadPool;

	std::atomic<int> counter = 0;

	auto increment = [&counter]() {counter++; };

	for (int i = 0; i < 1000; i++)
	{
		threadPool.submit(increment);
	}

	ASSERT_EQ(counter, 1000);
}

