#pragma once
#include <utility>
#include "LinearSet.h"
#include "Optional.h"
#include "RegexState.h"

namespace std
{
	template<typename T, typename V>
	struct hash<std::pair<T, V>>
	{
		size_t operator() (const std::pair<T, V>& Value) const
		{
			return hash<T>()(Value.first) ^ hash<V>()(Value.second);
		}
	};
}
