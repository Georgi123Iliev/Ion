#pragma once

#include "Net/TCP/TcpConnection.hpp"
#include "Net/HTTP/HttpTypes.hpp"

namespace Ion::Net::HTTP
{
	struct HttpContext
	{
		TCP::TcpConnection& conn;
		const HttpHead& httpHead;
		std::span<const std::byte> spillover;
	};
}
