#pragma once
#include "pch.h"

#include<array>
#include<string_view>
#include<span>

#include "Parsing/ParsingWrapper.hpp"
#include "Threading/ThreadPool.hpp"
#include "Net/NetworkEnvironment.hpp"
#include "Net/TCP/TcpAcceptor.hpp"
#include "Net/HTTP/HttpHead.hpp"
#include "Net/HTTP/Router.hpp"
#include "Net/HTTP/RequestReader.hpp"

#include "Utility/Conversions.hpp"

#include "Net/Constants.hpp"
namespace Ion::Net::HTTP
{
	

	class HttpServer
	{
	public:
		void actionLoop()
		{
			while (true)
			{

				auto action = [&router = std::as_const(m_router),connResult = std::move(m_connectionAcceptor.accept())]() mutable {
					

					using namespace Ion::Parsing;


					

					if (!connResult)
					{
						//log?
						return;
					}

					auto& conn = *connResult;

					//Initiate parsing

					//making buffer for the incoming bytes - a single frame
					std::array<std::byte, Constants::BUFFER_SIZE> buffer = {};

					std::span<std::byte> bufferSpan { buffer };

					//set up request reader
					RequestReader requestReader(conn);

					//recv considers getting zero bytes an error - a graceful shutdown
					while (auto recvResult = conn.recv(bufferSpan))
					{


						std::span<std::byte> readBytes = recvResult.value();

						auto readResult = requestReader.read(readBytes, bufferSpan);

						if (!readResult)
						{
							return;
						}

						auto& request = *readResult;

						auto handler = router.route(request.httpHead);

						//handlers handle the request and return the point from where parsing should resume
						auto excessSpillover = handler(conn, request.httpHead, request.spillover);

						std::memmove(buffer.data(), excessSpillover.data(), excessSpillover.size());

						
						

					}

					//recv failed - some error respone is to be issued

					
				};

				m_threadPool.dispatch(action);

			}
		}


	private:
		//router
		// 
		//parser is stateless
		//

		Router m_router;
		Threading::ThreadPool m_threadPool;
		Net::TCP::TcpAcceptor m_connectionAcceptor;
		


	};

	


}