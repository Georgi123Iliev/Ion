#pragma once

#include <cstdarg>

namespace Ion::Net::Types
{
	// Fits a SOCKET and an int
	using OpaqueHandle = std::uintptr_t;
}