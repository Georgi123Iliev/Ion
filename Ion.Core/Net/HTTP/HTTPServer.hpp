#pragma once

#include <array>
#include <string_view>
#include <span>

#include "Threading/ThreadPool.hpp"
#include "Net/NetworkEnvironment.hpp"
#include "Net/TCP/TcpAcceptor.hpp"
#include "Net/HTTP/Routing/Router.hpp"

namespace Ion::Net::HTTP
{
	class HttpServer
	{
	public:
		HttpServer(const NetworkEnvironment& env, Router<>&& router)
			: m_router(std::move(router)), env(&env), m_connectionAcceptor(env)
		{
		}

		void run();

		

	private:
		Router<> m_router;
		Threading::ThreadPool m_threadPool;
		const NetworkEnvironment* env;
		Net::TCP::TcpAcceptor m_connectionAcceptor;
	};
}