#pragma once

#include <string>
#include <string_view>
#include <array>

#include "Net/Constants.hpp"

namespace Ion::Net::HTTP
{
	enum class StatusCode
	{
		SUCCESS = 200,
		ERROR = 400,
		NOT_FOUND = 404
	};

	struct header
	{
		std::string_view name;
		std::string_view value;
	};
	

	

	struct HttpResponseHead
	{

		StatusCode statusCode;
		std::string responseMsg;
		int minorVersion;
		std::array<header, Ion::Net::Constants::MAX_HEADERS> headers;


	};

}

