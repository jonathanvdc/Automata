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

State NameSets(LinearSet<State> vals)
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
}

template<typename T>
T id(T Value)
{
	return Value;
}

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cout << "No arguments have been specified. Give me something to do:" << std::endl;
		std::cout << " * ssc <input file>.nfa <output file>.dfa" << std::endl;
		std::cout << " * mssc <input file>.enfa <output file>.dfa" << std::endl;
		std::cout << " * accepts <input file> <string>" << std::endl;
		return 0;
	}

	AutomatonParser parser;

	std::ifstream input(argv[2]);

	if (!input)
	{
		std::cout << "Input file '" << argv[2] << "' could not be opened." << std::endl;
		return 0;
	}

	if (std::string(argv[1]) == "ssc")
	{
		auto nfa = parser.ReadNFAutomaton(input);
		input.close();
		auto dfa = nfa.ToDFAutomaton();

		auto setRenamer = Automata::LambdaFunction<LinearSet<State>, State>(NameSets);
		auto charRenamer = Automata::LambdaFunction<Symbol, Symbol>(id<Symbol>);

		auto renamedDfa = dfa.Rename(&setRenamer, &charRenamer);

		std::ofstream output(argv[3]);

		parser.Write(renamedDfa, output);

		output.close();
	}
	else if (std::string(argv[1]) == "mssc")
	{
		auto enfa = parser.ReadENFAutomaton(input);
		input.close();
		auto dfa = enfa.ToDFAutomaton();

		auto setRenamer = Automata::LambdaFunction<LinearSet<State>, State>(NameSets);
		auto charRenamer = Automata::LambdaFunction<Symbol, Symbol>(id<Symbol>);

		auto renamedDfa = dfa.Rename(&setRenamer, &charRenamer);

		std::ofstream output(argv[3]);

		parser.Write(renamedDfa, output);

		output.close();
	}
	else
	{
		auto automaton = parser.ReadAutomaton(input);

		std::vector<std::string> parsedString;
		for (char item : std::string(argv[3]))
		{
			parsedString.push_back(std::string(1, item));
		}
		
		if (automaton->Accepts(parsedString))
		{
			std::cout << "The automaton accepts the given string." << std::endl;
		}
		else
		{
			std::cout << "The automaton did not accept the given string." << std::endl;
		}
	}

	return 0;
}

