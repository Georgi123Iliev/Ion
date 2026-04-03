#pragma once

#include <string_view>
#include <span>

#include "picohttpparser.h"

namespace Ion::Net::HTTP::Parsing
{
	struct ParseResult
	{
		int result;

		std::string_view wholeMsg;
		std::string_view method;
		std::string_view path;
		int minorVersion;
		std::span<phr_header> headers;
		size_t lastLen;
	};

	ParseResult parse(std::string_view msg, const ParseResult& lastParse);

	ParseResult parse(std::string_view msg, std::span<phr_header> headers);
}
