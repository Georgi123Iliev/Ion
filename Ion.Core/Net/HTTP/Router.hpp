#pragma once

#include <map>
#include <span>
#include <string_view>
#include <concepts>


#include "Net/Types.hpp"
#include "Net/HTTP/HttpHead.hpp"
#include "Net/TCP/TcpConnection.hpp"





namespace Ion::Net::HTTP
{

	using HttpHandler = std::span<const std::byte>(*)(TCP::TcpConnection& conn, const HttpHead& httpHead, std::span<const std::byte> spillover);

	using Path = std::string_view;

	using HandlerID = std::pair<Types::Method, Path>;

	template<typename RoutingTable>
	requires requires(RoutingTable rt)
	{
		{ rt.find(std::declval<HandlerID>()) } -> std::same_as<HttpHandler>;
		{ rt.at(std::declval<HandlerID>()) } -> std::same_as<HttpHandler>;
	}
	class Router
	{

		

		public:

			Router()
			{

			}

			HttpHandler route(const HttpHead& head) const
			{



				return HttpHandler();
			}
		private:


			RoutingTable m_routingTable;

			HttpHandler m_notFound; //fallback if no match

	};

}
