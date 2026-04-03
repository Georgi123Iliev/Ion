#include "pch.h"

#include "Utility/Conversions.hpp"
#include "RequestReader.hpp"

namespace Ion::Net::HTTP
{
	RequestReader::RequestReader(TCP::TcpConnection& conn)
		: m_phr_headers(), conn(conn)
	{
	}

	[[nodiscard]]
	std::expected<HttpRequest, std::error_code>
	RequestReader::read(std::span<std::byte> toParse, std::span<std::byte> entireFrame)
	{
		std::string_view stringBuffer = Utility::toStringView(toParse);

		auto parseRes = Parsing::parse(stringBuffer, std::span{ m_phr_headers });

		while (parseRes.result == -2)
		{
			auto remainingBuffer = entireFrame.subspan(toParse.size());

			auto recvResult = conn.recv(remainingBuffer);

			if (!recvResult)
			{
				return std::unexpected(std::error_code());
			}

			auto recvBytes = recvResult.value();

			toParse = entireFrame.subspan(0, toParse.size() + recvBytes.size());

			stringBuffer = Utility::toStringView(toParse);

			parseRes = Parsing::parse(stringBuffer, parseRes);
		}

		if (parseRes.result == -1)
		{
			return std::unexpected(std::error_code());
		}

		auto spillover = entireFrame.subspan(parseRes.result);

		return HttpRequest{ HttpHead(parseRes), spillover };
	}
}