#pragma once
#include<stdexcept>
#include<system_error>

namespace Ion::Net
{
	//only semantically different to system error
	class NetworkException : public std::system_error {
	public:
		using std::system_error::system_error;
	};
}

