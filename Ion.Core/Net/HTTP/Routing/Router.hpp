#pragma once

#include <unordered_map>
#include <span>
#include <cstddef>
#include <utility>

#include "Net/HTTP/Types/HttpTypes.hpp"
#include "Net/HTTP/Routing/DefaultHandlers.hpp"
#include "Net/TCP/TcpConnection.hpp"

namespace Ion::Net::HTTP
{
	using HttpHandler = std::span<const std::byte>(*)(
		TCP::TcpConnection& conn,
		const HttpHead& httpHead,
		std::span<const std::byte> spillover
	);

	using HandlerID = std::pair<HttpMethod, Path>;

	struct HandlerIDHasher
	{
		std::size_t operator()(const HandlerID& h) const noexcept
		{
			const auto& [method, path] = h;

			std::size_t h1 = std::hash<HttpMethod>{}(method);
			std::size_t h2 = std::hash<Path>{}(path);
			return h1 ^ (h2 << 1); 
		}
	};

	template <typename T>
	concept is_routing_table = is_map_like<T, HandlerID, HttpHandler>;

	using DefaultRoutingTable = std::unordered_map<HandlerID, HttpHandler, HandlerIDHasher>;

	static_assert(is_routing_table<DefaultRoutingTable>);

	template <typename RoutingTable = DefaultRoutingTable>
	requires is_routing_table<RoutingTable>
	class Router
	{
	public:
		Router() = default;

		Router(RoutingTable&& table)
			: m_routingTable(std::move(table))
		{
		}

		[[nodiscard]]
		HttpHandler route(const HttpHead& head) const noexcept
		{
			HandlerID id = { head.method, head.path };

			auto it = m_routingTable.find(id);
			if (it != m_routingTable.end())
			{
				return it->second;
			}

			return m_notFound;
		}

	private:
		RoutingTable m_routingTable;
		HttpHandler m_notFound{ notFound };
	};
}
