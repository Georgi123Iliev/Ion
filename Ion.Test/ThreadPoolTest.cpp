#include "pch.h"

#include "Threading/ThreadPool.h"


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

	steady_clock::time_point before;
	unsigned int workerCount = 1;


	{
		ThreadPool threadPool;

		workerCount = threadPool.getWorkerCount();

		before = high_resolution_clock::now();

		for (int i = 0; i < 30; i++)
		{
			threadPool.submit(timeConsumingTask);
		}

		


	}

	auto after = high_resolution_clock::now();

	auto elapsed = after - before;

	
	// Can't get perfect perfomance due to overhead in access of the shared resource(queue)
	// so we multiply by 1.5 to give some leeway
	ASSERT_LT(elapsed, (30s / workerCount)*1.5);
}

TEST(ThreadPoolTest, Returns)
{
	using namespace std::chrono_literals;
		

	ThreadPool threadPool;

	auto returningAction = []() {return 42; };

	auto nonReturningAction = []() {};


	auto f = threadPool.submit(returningAction);

	int x = -1;

	if (f.valid())
	{
		x = f.get();
	}
	
	ASSERT_EQ(x, 42);


	/*Ignored result*/ threadPool.submit(returningAction);

	threadPool.submit(nonReturningAction);


	
	
}