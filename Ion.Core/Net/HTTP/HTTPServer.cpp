#include "pch.h"

#include <csignal>

#include "Net/NetworkEnvironment.hpp"
#include "Net/HTTP/RequestReader.hpp"
#include "Net/Constants.hpp"
#include "Net/HTTP/HttpServer.hpp"




void Ion::Net::HTTP::HttpServer::run()
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

		auto action = [&router = std::as_const(m_router), conn = std::move(*connResult)]() mutable {

			using namespace Ion::Net::HTTP::Parsing;

			std::array<std::byte, Constants::BUFFER_SIZE> buffer;

			std::span<std::byte> bufferSpan{ buffer };

			RequestReader requestReader(conn);

			while (auto recvResult = conn.recv(bufferSpan))
			{
				std::span<std::byte> bytesRemainingToParse = recvResult.value();

				do
				{
					auto readResult = requestReader.read(bytesRemainingToParse, bufferSpan);

					if (!readResult)
					{
						return;
					}

					auto& request = *readResult;

					auto handler = router.route(request.httpHead);

					auto excessSpillover = handler(conn, request.httpHead, request.spillover);

					std::memmove(buffer.data(), excessSpillover.data(), excessSpillover.size());

					bytesRemainingToParse = std::span{ buffer.data(), excessSpillover.size() };

				} while (!bytesRemainingToParse.empty());
			}
		};

		m_threadPool.dispatch(std::move(action));
	}
}
