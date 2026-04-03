#pragma once
#include <span>
#include "Net/HTTP/Types/HttpTypes.hpp"
namespace Ion::Net::HTTP
{
	

	struct HttpRequest
	{
		HttpHead httpHead;
		std::span<std::byte> spillover;

		HttpRequest(const HttpHead& head, std::span<std::byte> spillover)
			:httpHead(head), spillover(spillover)
		{

		}

	};
}

