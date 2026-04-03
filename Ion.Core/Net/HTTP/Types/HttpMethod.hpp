#pragma once

#include <string_view>

namespace Ion::Net::HTTP
{
	struct HttpMethod
	{
		static const HttpMethod GET;
		static const HttpMethod POST;

		explicit HttpMethod(std::string_view method)
			: method(method)
		{
		}

		bool operator==(const HttpMethod& other) const
		{
			return method == other.method;
		}

		std::string_view method;
	};

	inline const auto HttpMethod::GET = HttpMethod{ "GET" };
	inline const auto HttpMethod::POST = HttpMethod{ "POST" };
}

namespace std
{
	template <>
	struct hash<Ion::Net::HTTP::HttpMethod>
	{
		std::size_t operator()(const Ion::Net::HTTP::HttpMethod& m) const noexcept
		{
			return std::hash<std::string_view>{}(m.method);
		}
	};
}
