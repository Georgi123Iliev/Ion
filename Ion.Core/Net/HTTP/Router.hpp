#pragma once

#include <unordered_map>
#include <span>

#include <cstddef>

#include <utility>

#include "Net/HTTP/HttpTypes.hpp"
#include "Net/TCP/TcpConnection.hpp"






namespace Ion::Net::HTTP
{
	

	/*
	using HttpHandler = std::function<
		std::span<const std::byte>(
			
		)>;
		*/
	std::span<const std::byte> notFound(TCP::TcpConnection& conn,
		const HttpHead& httpHead,
		std::span<const std::byte> spillover);
		

	
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
			return h1 ^ (h2 << 1); // or use boost::hash_combine
		}
	};

	template<typename T>
	concept is_routing_table = is_map_like<T, HandlerID, HttpHandler>;

	using DefaultRoutingTable = std::unordered_map<HandlerID, HttpHandler, HandlerIDHasher>;

	static_assert(is_routing_table<DefaultRoutingTable>);


	template<typename RoutingTable = std::unordered_map<HandlerID,HttpHandler,HandlerIDHasher>>
	requires is_routing_table<RoutingTable>
	class Router
	{

		

		public:

			Router() = default;

			Router(RoutingTable&& table)
				:m_routingTable(std::forward<RoutingTable&&>(table))
			{

			}

			[[nodiscard]] 
			HttpHandler route(const HttpHead& head) const noexcept
			{
				HandlerID id = 
				{head.method,head.path};

				

				if (m_routingTable.find(id) != m_routingTable.end())
				{
					return m_routingTable.at(id);
				}

				return m_notFound;
			}
		private:


			RoutingTable m_routingTable;

			HttpHandler m_notFound{notFound}; //fallback if no match

	};

	
	

}
