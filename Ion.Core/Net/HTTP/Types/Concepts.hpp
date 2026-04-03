#pragma once

#include <type_traits>

namespace Ion::Net::HTTP
{
	template <typename T, typename keyT, typename valT>
	concept is_map_like =
		requires(T t, keyT k)
	{
		{ t.find(k) != t.end() } -> std::convertible_to<bool>;
		{ std::remove_reference_t<decltype(t.find(k)->second)>() } -> std::same_as<valT>;
	};



	template <typename T, typename keyT, typename valT>
	concept map_like_insertion =
		requires(T t, keyT k, valT v)
	{
		{ t[k] = v };
	};


}
