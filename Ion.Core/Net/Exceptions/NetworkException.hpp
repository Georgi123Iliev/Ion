#pragma once
#include<stdexcept>
#include<system_error>
class NetworkException : std::runtime_error
{
public:
	NetworkException(const char* msg, std::error_code err)
		: std::runtime_error(msg), err(err)
	{}

	auto getErr() const noexcept
	{
		return err;
	}

private:
	std::error_code err;

};

