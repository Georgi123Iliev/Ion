#include "pch.h"
#include "Conversions.hpp"

std::string_view Ion::Utility::toStringView(std::span<const std::byte> bytes)
{
	return { reinterpret_cast<const char*>(bytes.data()),bytes.size() };
}

std::span<const std::byte> Ion::Utility::toByteSpan(std::string_view sv)
{
	return { reinterpret_cast<const std::byte*>(sv.data()),sv.size() };
}
