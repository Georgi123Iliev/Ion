#include "pch.h"

#include <string_view>
#include <span>

#include "ParsingWrapper.hpp"

namespace Ion::Net::HTTP::Parsing
{
	ParseResult makeDefaultParseResult(std::span<phr_header> headers)
	{
		ParseResult pr =
		{
			.headers{headers},
			.lastLen = 0z
		};

		return pr;
	}

	ParseResult parse(std::string_view msg, const ParseResult& lastParse)
	{
		const char* method_ptr = lastParse.method.data();
		size_t method_len = lastParse.method.length();

		const char* path_ptr = lastParse.path.data();
		size_t path_len = lastParse.path.length();

		struct phr_header* headers_internal = lastParse.headers.data();

		//static_assert(false, "MISTAKE SHRINKING HEADER ON INCOMPLETE READ");

		size_t num_headers = lastParse.headers.size();

		size_t last_len = lastParse.lastLen;

		int minor_version = lastParse.minorVersion;

		int result = phr_parse_request(msg.data(), msg.length(),
			&method_ptr, &method_len, &path_ptr, &path_len,
			&minor_version, headers_internal, &num_headers, last_len);

		ParseResult pr =
		{
			.result = result,
			.wholeMsg = msg,
			.method = std::string_view{method_ptr, method_len},
			.path = std::string_view{path_ptr, path_len},
			.minorVersion = minor_version,
			.headers = std::span{headers_internal, num_headers},
			.lastLen = msg.length()
		};

		return pr;
	}

	ParseResult parse(std::string_view msg, std::span<phr_header> headers)
	{
		auto defaultParseRes = makeDefaultParseResult(headers);

		return parse(msg, defaultParseRes);
	}
}