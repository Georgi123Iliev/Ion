#pragma once

#include <string_view>

namespace Ion::Net::HTTP
{
	struct Header
	{
		std::string_view name;
		std::string_view value;
	};
}
