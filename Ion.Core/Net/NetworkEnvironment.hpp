#pragma once
#include<memory>
#include<atomic>
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
	std::atomic<bool> m_exists{0};
	class EnvironmentImpl;
	std::unique_ptr<EnvironmentImpl> m_impl;
};

