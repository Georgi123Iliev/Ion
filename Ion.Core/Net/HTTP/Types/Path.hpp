#pragma once

#include <string_view>

namespace Ion::Net::HTTP
{
	struct Path
	{
		Path(std::string_view path)
			: path(path)
		{
		}

		bool operator==(const Path& other) const
		{
			return path == other.path;
		}

		std::string_view path;
	};
}

namespace std
{
	template <>
	struct hash<Ion::Net::HTTP::Path>
	{
		std::size_t operator()(const Ion::Net::HTTP::Path& p) const noexcept
		{
			return std::hash<std::string_view>{}(p.path);
		}
	};
}
