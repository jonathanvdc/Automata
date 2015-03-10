// Automata.cpp : Defines the entry point for the console application.
//

#include <unordered_map>
#include <memory>
#include <iostream>
#include <cassert>
#include <string>
#include <set>
#include <sstream>
#include <fstream>
#include "DFAutomaton.h"
#include "NFAutomaton.h"
#include "ENFAutomaton.h"
#include "ArraySlice.h"
#include "TransitionTable.h"
#include "AutomatonParser.h"
#include "HashExtensions.h"
#include "LambdaFunction.h"

typedef std::string State;
typedef std::string Symbol;

typedef Automata::DFAutomaton<State, Symbol> DFA;
typedef Automata::NFAutomaton<State, Symbol> NFA;

int main(int argc, const char* argv[])
{
	AutomatonParser parser;

	std::ifstream input(argv[1]);

	auto nfa = parser.ReadNFAutomaton(input);

	input.close();

	auto dfa = nfa.ToDFAutomaton(LinearSet<std::string>({ "0", "1" }));

	for (size_t i = 0; i < 25; i++)
	{
		stdx::ArraySlice<Symbol> str(i);
		for (size_t j = 0; j < i; j++)
		{
			str[j] = "0";
		}
		assert(dfa.Accepts(str) == nfa.Accepts(str));
	}

	assert(!dfa.Accepts(stdx::ArraySlice<Symbol>({ "0", "1", "0", "0", "0", "1" })));
	assert(!nfa.Accepts(stdx::ArraySlice<Symbol>({ "0", "1", "0", "0", "0", "1" })));

	auto setRenamer = Automata::LambdaFunction<LinearSet<State>, State>([](LinearSet<State> vals)
	{
		auto valsVec = vals.getItems();
		std::set<State> states(valsVec.begin(), valsVec.end());
		std::ostringstream result;
		result << "{";
		bool first = true;
		for (auto& item : states)
		{
			if (!first)
			{
				result << ",";
			}
			first = false;
			result << item;
		}
		result << "}";
		return result.str();
	});
	auto charRenamer = Automata::LambdaFunction<Symbol, Symbol>([](Symbol val) { return val; });

	auto renamedDfa = dfa.Rename(&setRenamer, &charRenamer);

	std::ofstream output(argv[2]);

	parser.Write(renamedDfa, output);

	output.close();

	return 0;
}

