#pragma once
#include <array>
#include <span>

#include "Net/TCP/TcpConnection.hpp"
#include "Net/HTTP/RequestReader.hpp"
#include "Net/HTTP/Routing/Router.hpp"

#include <iostream>
namespace Ion::Net::HTTP
{
	template<typename Router>
	//requires Router is an instance of Router<T>
	class HttpSession
	{

	public:
		HttpSession(TCP::TcpConnection&& conn, const Router& router)
			:m_conn(std::move(conn)),m_requestReader(m_conn),m_router(router)
		{

		}
	
		auto serve()
		{
			using namespace Ion::Net::HTTP::Parsing;

			std::span<std::byte> frameSpan{ m_frame };
			
			while (true)
			{
				auto recvResult = m_conn.recv(frameSpan);

				if (!recvResult)
				{
					std::cout << "\nERROR\n" << recvResult.error() << "\nERROR\n";

					break;
					//return recvResult.error();
				}
				

				std::span<std::byte> bytesRemainingToParse = recvResult.value();

				do
				{
					auto readResult = m_requestReader.read(bytesRemainingToParse, frameSpan);

					if (!readResult)
					{
						return;
					}

					auto& request = *readResult;

					auto handler = m_router.route(request.httpHead);

					auto excessSpillover = handler(m_conn, request.httpHead, request.spillover);

					std::memmove(m_frame.data(), excessSpillover.data(), excessSpillover.size());

					bytesRemainingToParse = std::span{ m_frame.data(), excessSpillover.size() };

				} while (!bytesRemainingToParse.empty());
			}

			
		}


	private:

		std::array<std::byte, Constants::BUFFER_SIZE> m_frame = {};
		
		TCP::TcpConnection m_conn;
		RequestReader m_requestReader;
		const Router& m_router;
		
	};

}