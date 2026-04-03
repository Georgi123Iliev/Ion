#pragma once

#include <string_view>
#include <span>

namespace Ion::Utility
{
	std::string_view toStringView(std::span<const std::byte> bytes);

	std::span<const std::byte> toByteSpan(std::string_view sv);
}