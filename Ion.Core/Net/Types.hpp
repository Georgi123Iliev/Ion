#pragma once

#include <cstdint>

namespace Ion::Net::Types
{
	// Fits a SOCKET and an int
	using OpaqueHandle = std::uintptr_t;
}