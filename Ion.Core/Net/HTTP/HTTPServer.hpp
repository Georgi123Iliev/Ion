#pragma once

#include<array>
#include<string_view>
#include<span>


#include "Threading/ThreadPool.hpp"
#include "Net/NetworkEnvironment.hpp"
#include "Net/TCP/TcpAcceptor.hpp"
#include "Net/HTTP/Router.hpp"


namespace Ion::Net::HTTP
{
	
	

	
	class HttpServer
	{
	public:

		HttpServer(const NetworkEnvironment& env,Router<>&& router)
			:  m_router(std::forward<Router<>&&>(router)),env(&env),m_connectionAcceptor(env)
		{

		}

		


		void actionLoop();


	private:
		 
		//parser is stateless so no field
		//

		Router<> m_router;
		Threading::ThreadPool m_threadPool;
		const NetworkEnvironment* env;
		Net::TCP::TcpAcceptor m_connectionAcceptor;


		


	};

	


}