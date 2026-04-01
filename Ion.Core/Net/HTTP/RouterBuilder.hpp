#pragma once
#include <concepts>
#include "Net/HTTP/Router.hpp"

namespace Ion::Net::HTTP
{
	


	template<typename T = DefaultRoutingTable>
	requires is_routing_table<T>
	class RouterBuilder
	{
	public:
		RouterBuilder& addStaticRoute(HttpMethod method, Path path, HttpHandler handler)
		{
			m_internalTable[{method, path}] = handler;

			return *this;
		}

		Router<T> build() const
		{
			return { m_internalTable };
		}

		Router<T> consume()
		{
			return { std::move(m_internalTable) };
		}

	private:
		T m_internalTable = {};
	};
}

