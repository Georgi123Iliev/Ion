#include<string_view>
#include<span>
#include<bit>

namespace Ion::Utility
{

	std::string_view toStringView(std::span<const std::byte> bytes)
	{
		return { reinterpret_cast<const char*>(bytes.data()),bytes.size() };
	}
}