#pragma once

#include <span>
#include <ranges>
#include <string_view>

#include "Net/HTTP/Types/Header.hpp"
#include "Net/HTTP/Types/HttpMethod.hpp"
#include "Net/HTTP/Types/Path.hpp"
#include "Net/HTTP/Parsing/ParsingWrapper.hpp"

namespace Ion::Net::HTTP
{
	struct HttpHead
	{
		HttpMethod method;
		Path path;
		int minorVersion;
		std::string_view entireHead;

		explicit HttpHead(const Ion::Net::HTTP::Parsing::ParseResult& pr)
			: method(pr.method), path(pr.path),
			minorVersion(pr.minorVersion), entireHead(pr.wholeMsg.substr(0, pr.result)),
			m_raw_headers(pr.headers)
		{
		}

		auto headers() const
		{
			auto to_internal = [](const auto& h) {
				return Header
				{
					.name = {h.name, h.name_len},
					.value = {h.value, h.value_len}
				};
			};

			return m_raw_headers | std::views::transform(to_internal);
		}

	private:
		std::span<phr_header> m_raw_headers;
	};
}
