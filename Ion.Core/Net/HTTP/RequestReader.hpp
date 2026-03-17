#pragma once

#include<span>
#include<expected>

#include "Parsing/ParsingWrapper.hpp"
#include "Utility/Conversions.hpp"
#include "Net/HTTP/HttpHead.hpp"
#include "Net/TCP/TcpConnection.hpp"
#include "Net/Types.hpp"


namespace Ion::Net::HTTP
{

	struct Request
	{
		HttpHead httpHead;
		std::span<std::byte> spillover;

		Request(const HttpHead& head, std::span<std::byte> spillover)
			:httpHead(head),spillover(spillover)
		{

		}

	};

	class RequestReader
	{


	public:

		RequestReader(TCP::TcpConnection& conn)
			: conn(conn)
		{

		}


		std::expected<Request,std::error_code> read(std::span<std::byte> toParse, std::span<std::byte> entireFrame)
		{


			
			std::string_view stringBuffer = toStringView(toParse);

			auto parseRes = Parsing::parse(stringBuffer, std::span{ m_phr_headers });



			while (parseRes.result == -2)
			{



				auto remainingBuffer = entireFrame.subspan(toParse.size());

				//appending newly read bytes to the buffer

				auto recvResult = conn.recv(remainingBuffer);

				if (!recvResult)
				{	//log
					return;
				}

				auto recvBytes = recvResult.value();

				toParse = entireFrame.subspan(0, toParse.size() + recvBytes.size());

				stringBuffer = toStringView(toParse);

				//stringBuffer already points to buffer
				//parseRes remembers where the parsing reached so it doesnt reparse needlessly
				parseRes = parse(stringBuffer, parseRes);
			}

			if (parseRes.result == -1)
			{
				//log
				return;
			}


			auto spillover = entireFrame.subspan(toParse.size());

			return Request{HttpHead(parseRes),spillover};

		}
	private:
		std::array<phr_header,Ion::Net::Constants::MAX_HEADERS> m_phr_headers;
		TCP::TcpConnection& conn;



	};
}

