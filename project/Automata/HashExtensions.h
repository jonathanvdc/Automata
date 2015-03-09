#pragma once
#include "../../bin/Pair.h"
#include "../../bin/LinearSet.h"

namespace std
{
	template<typename T, typename V>
	struct hash<Pair<T, V>>
	{
		size_t operator()(const Automata::Pair<T, V>& Value) const
		{
			return hash<T>()(Value.getFirst()) ^ hash<V>()(Value.getSecond());
		}
	};

	template<typename T>
	struct hash<LinearSet<T>>
	{
		size_t operator()(const LinearSet<T>& Value) const
		{
			size_t result = 0;
			for (auto item : Value.getItems())
			{
				result ^= hash<T>()(item);
			}
			return result;
		}
	};
}