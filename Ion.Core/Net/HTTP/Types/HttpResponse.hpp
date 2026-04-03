#pragma once

#include <string>
#include <array>

#include "Net/Constants.hpp"
#include "Net/HTTP/Types/Header.hpp"
#include "Net/HTTP/Types/StatusCode.hpp"

namespace Ion::Net::HTTP
{
	struct HttpResponseHead
	{
		StatusCode statusCode;
		std::string responseMsg;
		int minorVersion;
		std::array<Header, Ion::Net::Constants::MAX_HEADERS> headers;
	};
}
