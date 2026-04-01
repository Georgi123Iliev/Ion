#pragma once

#include<span>
#include<expected>

#include "Parsing/ParsingWrapper.hpp"
#include "Net/HTTP/HttpRequest.hpp"
#include "Net/TCP/TcpConnection.hpp"
#include "Net/Types.hpp"


namespace Ion::Net::HTTP
{

	class RequestReader
	{


	public:

		RequestReader(TCP::TcpConnection& conn);


		std::expected<HttpRequest,std::error_code> read(std::span<std::byte> toParse, std::span<std::byte> entireFrame);
	private:
		std::array<phr_header,Ion::Net::Constants::MAX_HEADERS> m_phr_headers;
		TCP::TcpConnection& conn;



	};
}

