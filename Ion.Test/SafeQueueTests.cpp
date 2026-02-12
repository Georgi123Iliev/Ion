#include "pch.h"

#include <thread>
#include <chrono>
#include <array>

#include "Threading/SafeQueue.h"

using namespace Ion::Threading;

TEST(SafeQueueTest, SanityCheckFIFO) {
  
	SafeQueue<int> safeQueue;

	safeQueue.push(1);
	safeQueue.push(2);
	safeQueue.push(3);

	auto val1 = safeQueue.pop();
	ASSERT_TRUE(val1.has_value());
	EXPECT_EQ(val1.value(), 1);

	auto val2 = safeQueue.pop();
	ASSERT_TRUE(val2.has_value());
	EXPECT_EQ(val2.value(), 2);

	auto val3 = safeQueue.pop();
	ASSERT_TRUE(val3.has_value());
	EXPECT_EQ(val3.value(), 3);
}

TEST(SafeQueueTest, WakeUp)
{
	using namespace std::chrono_literals;

	SafeQueue<int> safeQueue;

	std::atomic<bool> hasPopped;

	{
		std::jthread background([&safeQueue, &hasPopped]()
			{
				auto val = safeQueue.pop();

				hasPopped = true;
			}
		);

		std::this_thread::sleep_for(100ms);

		ASSERT_FALSE(hasPopped);

		safeQueue.push(42);
	}

	ASSERT_TRUE(hasPopped);

}

TEST(SafeQueueTest, ShutdownEmptyQueue)
{
	using namespace std::chrono_literals;

	SafeQueue<int> safeQueue;

	std::atomic<bool> wasNullOpt = false;

	{
		std::jthread background([&safeQueue,&wasNullOpt]()
			{
				auto val = safeQueue.pop();

				wasNullOpt = (val == std::nullopt);
				
			}
		);

		std::this_thread::sleep_for(100ms);

		safeQueue.shutdown();

	}

	ASSERT_TRUE(wasNullOpt);

}

TEST(SafeQueueTest, ShutdownFullQueue)
{
	using namespace std::chrono_literals;

	SafeQueue<int> safeQueue;

	std::array<int,5> dataset = { 1,2,3,4,5 };

	
	//fill the queue with data
	std::for_each(dataset.cbegin(), dataset.cend(), [&safeQueue](const auto& v) {safeQueue.push(v); });

	safeQueue.shutdown(); //shutdown with data in queue
	
	// same test with all but the last one - we should get a value every time here
	for(int i = 0; i < dataset.size(); i++)
	{
		std::atomic<bool> wasNullOpt = false;

		{

			std::jthread background([&safeQueue, &wasNullOpt]()
				{
					auto val = safeQueue.pop();

					wasNullOpt = val == std::nullopt;

				}
			);
		}

		ASSERT_FALSE(wasNullOpt);

	}

	//last pop should be nullopt

	std::atomic<bool> wasNullOpt = false;

	{
		std::jthread background([&safeQueue, &wasNullOpt]()
			{
				auto val = safeQueue.pop();

				wasNullOpt = val == std::nullopt;

			}
		);
	}
	ASSERT_TRUE(wasNullOpt);
}

TEST(SafeQueueTest, PushAfterShutdown)
{
	SafeQueue<int> safeQueue;

	safeQueue.shutdown();

	EXPECT_THROW(safeQueue.push(42), std::runtime_error);

	
}

TEST(SafeQueueTest, StressTest)
{
	SafeQueue<int> safeQueue;

	std::atomic<int> totalItemCount = 0;
	auto producerAction = [&safeQueue, &totalItemCount]() { 
		for (int i = 1; i <= 100; i++)
		{
			safeQueue.push(i);

			totalItemCount++;
		}
		
	};


	
	auto consumerAction = [&safeQueue, &totalItemCount]() {
		for (int i = 1; i <= 100; i++)
		{
			if (safeQueue.pop())
				totalItemCount--;
		}

	};

	{
		std::vector<std::jthread> producers;
		producers.reserve(10);

		std::vector<std::jthread> consumers;
		consumers.reserve(10);

		for (int i = 0; i < 10; i++)
		{

			producers.emplace_back(producerAction);

			consumers.emplace_back(consumerAction);
		}
	}

	ASSERT_EQ(totalItemCount, 0);

}
