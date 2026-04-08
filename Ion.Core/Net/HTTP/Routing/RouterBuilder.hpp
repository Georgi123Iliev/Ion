#pragma once

#include "Net/HTTP/Routing/Router.hpp"
#include "Net/HTTP/Types/Concepts.hpp"
namespace Ion::Net::HTTP
{
	
	template<typename T>
	concept insertion_in_routing_table = map_like_insertion<T, HandlerID, HttpHandler>;

	static_assert(insertion_in_routing_table<DefaultRoutingTable>);


	template<typename T = DefaultRoutingTable>
	requires is_routing_table<T> && insertion_in_routing_table<T>
	class RouterBuilder
	{
	public:
		RouterBuilder& addStaticRoute(HttpMethod method, Path path, HttpHandler handler)
		{
			m_internalTable[HandlerID{ method,path }] = handler;

			return *this;
		}
		
		[[nodiscard]]
		Router<T> build() const
		{
			return { m_internalTable };
		}

		[[nodiscard]]
		Router<T> consume()
		{
			return { std::move(m_internalTable) };
		}

	private:
		T m_internalTable = {};
	};
}

