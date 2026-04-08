#pragma once

#include <array>
#include <string_view>
#include <span>

#include "Threading/ThreadPool.hpp"
#include "Net/NetworkEnvironment.hpp"
#include "Net/TCP/TcpAcceptor.hpp"

#include "Net/HTTP/HttpSession.hpp"

#include "Net/Constants.hpp"
namespace Ion::Net::HTTP
{
	template<typename RouterT>
	class HttpServer
	{
	public:
		HttpServer(const NetworkEnvironment& env, RouterT&& router)
			: m_router(std::move(router)), env(&env), m_connectionAcceptor(env)
		{
		}

		void run();

		

	private:
		RouterT m_router;
		Threading::ThreadPool m_threadPool;
		/*
		* env must preceed m_connectionAcceptor
		*/
		const NetworkEnvironment* env;
		Net::TCP::TcpAcceptor m_connectionAcceptor;
	};

	template<typename RouterT>
	inline void HttpServer<RouterT>::run()
	{
		while (true)
		{
			auto connResult = m_connectionAcceptor.accept();

			if (!connResult)
			{
				if (m_connectionAcceptor.isInterrupted())
					break;

				continue;
			}

			HttpSession httpSession{ std::move(connResult.value()),m_router };


			auto action = [httpSession = std::move(httpSession)] mutable {
				httpSession.serve();
			};

			m_threadPool.dispatch(std::move(action));
		}
	}
}

