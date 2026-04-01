#pragma once
#include <cstdarg>
#include <system_error>

namespace Ion::Net::Types
{
	//fits a SOCKET and an int
	using OpaqueHandle = std::uintptr_t;

	enum class Method
	{
		GET,
		POST,
		UNKNOWN
	};

	enum class Error
	{
		GracefulShutdown,
		InvalidHttpParse,

	};

	struct NetError
	{
		/*
		NetError()
		{

		}

		Error error;
		std::error_code sys_error{};
		*/
	};

	

}