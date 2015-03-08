// Automata.cpp : Defines the entry point for the console application.
//

#include <unordered_map>
#include <memory>
#include <iostream>
#include <cassert>
#include "../../bin/DFAutomaton.h"
#include "../../bin/NFAutomaton.h"
#include "../../bin/ArraySlice.h"
#include "../../bin/UnorderedMapFunction.h"

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


int main(int argc, const char* argv[])
{
	std::unordered_map<Pair<char, char>, LinearSet<char>> transtable =
	{
		{ { 'a', '0' }, LinearSet<char>(std::vector<char>({ 'b' })) },
		{ { 'a', '1' }, LinearSet<char>() },
		{ { 'b', '0' }, LinearSet<char>(std::vector<char>({ 'a' })) },
		{ { 'b', '1' }, LinearSet<char>() }
	};
	auto transftn = std::make_shared<Automata::UnorderedMapFunction<Pair<char, char>, LinearSet<char>>>(transtable);

	Automata::NFAutomaton<char, char> nfa('a', stdx::ArraySlice<char>({ 'a' }), transftn);
	auto dfa = nfa.ToDFAutomaton(LinearSet<char>(std::vector<char>({ '0', '1' })));

	for (size_t i = 0; i < 25; i++)
	{
		stdx::ArraySlice<char> str(i);
		for (size_t j = 0; j < i; j++)
		{
			str[j] = '0';
		}
		assert(dfa->Accepts(str) == nfa.Accepts(str));
	}

	assert(!dfa->Accepts(stdx::ArraySlice<char>({ '0', '1', '0', '0', '0', '1' })));

	return 0;
}

