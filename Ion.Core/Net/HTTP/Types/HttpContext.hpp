#pragma once

#include <span>
#include <cstddef>

#include "Net/TCP/TcpConnection.hpp"
#include "Net/HTTP/Types/HttpHead.hpp"

namespace Ion::Net::HTTP
{
	struct HttpContext
	{
		TCP::TcpConnection& conn;
		const HttpHead& httpHead;
		std::span<const std::byte> spillover;
	};
}
