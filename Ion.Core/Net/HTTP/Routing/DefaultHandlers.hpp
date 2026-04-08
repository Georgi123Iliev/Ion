#pragma once

#include <span>
#include <cstddef>

namespace Ion::Net::TCP { class TcpConnection; } //forward declaration

namespace Ion::Net::HTTP
{
	struct HttpHead;

	std::span<const std::byte> notFound(TCP::TcpConnection& conn,
		const HttpHead& httpHead,
		std::span<const std::byte> spillover);
}
