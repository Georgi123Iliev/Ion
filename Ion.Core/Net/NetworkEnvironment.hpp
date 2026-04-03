#pragma once
#include <memory>
#include <atomic>
#include <csignal>
#include <thread>
#include <stop_token>


namespace Ion::Net
{

	

	class NetworkEnvironment
	{
	public:
		NetworkEnvironment();
		~NetworkEnvironment();


		NetworkEnvironment(const NetworkEnvironment&) = delete;
		NetworkEnvironment& operator=(const NetworkEnvironment&) = delete;
		NetworkEnvironment(NetworkEnvironment&&) = delete;
		NetworkEnvironment& operator=(NetworkEnvironment&&) = delete;

	private:



		inline static std::atomic<bool> m_exists {false};
		class EnvironmentImpl;
		std::unique_ptr<EnvironmentImpl> m_impl;

		
	};

	


}

