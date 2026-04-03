#pragma once

#include <cstdint>

namespace Ion::Net::HTTP
{
	enum class StatusCode
	{
		SUCCESS = 200,
		ERROR = 400,
		NOT_FOUND = 404
	};
}
