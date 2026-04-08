#pragma once

#include <string_view>
#include <filesystem>

namespace Ion::Net::HTTP
{
	namespace fs = std::filesystem;

	struct Path
	{
		Path(std::string_view path)
			: path(path)
		{
		}

		operator fs::path() const { return { path }; }

		bool operator==(const Path& other) const = default;

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
