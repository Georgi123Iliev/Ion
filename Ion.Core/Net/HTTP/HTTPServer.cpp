#include "pch.h"



#include "Net/NetworkEnvironment.hpp"
#include "Net/HTTP/RequestReader.hpp"
#include "Net/Constants.hpp"
#include "Net/HTTP/HttpServer.hpp"

void Ion::Net::HTTP::HttpServer::actionLoop()
{
	while (true)
	{


		auto connResult = m_connectionAcceptor.accept();

		if (!connResult)
		{
			//possibly some logging utility?
			continue;
		}



		auto action = [&router = std::as_const(m_router), conn = std::move(*connResult)]() mutable {


			using namespace Ion::Parsing;



			//Initiate parsing

			//making buffer for the incoming bytes - a single frame
			//intentionally not initialized
			std::array<std::byte, Constants::BUFFER_SIZE> buffer;

			std::span<std::byte> bufferSpan{ buffer };

			//set up request reader - it can do extra recv-s if necessary
			RequestReader requestReader(conn);

			//recv considers getting zero bytes an error - a graceful shutdown
			while (auto recvResult = conn.recv(bufferSpan))
			{

				std::span<std::byte> bytesRemainingToParse = recvResult.value();

				//we know there has been > 0 bytes received because recvResult was no error
				do
				{


					auto readResult = requestReader.read(bytesRemainingToParse, bufferSpan);

					if (!readResult)
					{
						return;
					}

					auto& request = *readResult;

					auto handler = router.route(request.httpHead);

					//handlers handle the request and return the point from where parsing should resume
					auto excessSpillover = handler(conn, request.httpHead, request.spillover);

					std::memmove(buffer.data(), excessSpillover.data(), excessSpillover.size());

					bytesRemainingToParse = std::span{ buffer.data(),excessSpillover.size() };

				} while (!bytesRemainingToParse.empty());




			}

			//recv failed - some error respone is to be issued


			};

		m_threadPool.dispatch(std::move(action));

	}
}
