#pragma once
#include<stdexcept>
#include<system_error>

namespace Ion::Net //::Exceptions?
{

	class NetworkException : std::runtime_error
	{
	public:
		NetworkException(const char* msg, std::error_code err)
			: std::runtime_error(msg), m_err(err)
		{
		}

		auto getErr() const noexcept
		{
			return m_err;
		}

	private:
		std::error_code m_err;

	};
}

