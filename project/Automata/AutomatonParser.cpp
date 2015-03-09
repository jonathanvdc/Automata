#include "AutomatonParser.h"
#include "HashExtensions.h"
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include <unordered_map>

using namespace Automata;

enum AutomatonType
{
	Deterministic,
	NonDeterministic,
	EpsilonNonDeterministic
};

AutomatonType ReadType(std::istream& Input)
{
	std::string val;
	Input >> val;
	if (val == "dfa") return Deterministic;
	else if (val == "nfa") return NonDeterministic;
	else return EpsilonNonDeterministic;
}

std::ostream& operator<<(std::ostream& Output, LinearSet<AutomatonParser::Symbol> Values)
{
	for (auto& item : Values.getItems())
	{
		Output << " " << item;
	}
	return Output;
}

std::istream& operator>>(std::istream& Input, LinearSet<AutomatonParser::Symbol>& Values)
{
	std::string line;
	std::getline(Input, line);
	std::istringstream strstream(line);
	std::string val;
	strstream >> val;
	while (val.size() > 0 && strstream)
	{
		Values.Add(val);
		strstream >> val;
	}
	return Input;
}

AutomatonParser::Automaton AutomatonParser::ReadAutomaton(std::istream& Input)
{
	auto type = ReadType(Input);
	if (type == Deterministic)
	{
		return std::make_unique<DFA>(ReadDFAutomaton(Input));
	}
	else
	{
		return std::make_unique<NFA>(ReadNFAutomaton(Input));
	}
}

AutomatonParser::DFA AutomatonParser::ReadDFAutomaton(std::istream& Input)
{
	std::string word;
	Input >> word;

	State initial;
	LinearSet<State> finalStates;
	std::unordered_map<Pair<State, Symbol>, State> trans;

	while (word.size() > 0 && Input)
	{
		if (word == "start")
		{
			Input >> initial;
		}
		else if (word == "accepts")
		{
			Input >> finalStates;
		}
		else if (word == "transitions")
		{
			State a;
			Input >> a;
			while (a.size() > 0 && Input)
			{
				State b;
				Symbol c;
				Input >> c >> b;
				trans[Pair<State, Symbol>(a, c)] = b;
				Input >> a;
			}
		}
		else if (word != "dfa")
		{
			throw std::invalid_argument("Word '" + word + "' was not recognized while parsing a dfa.");
		}
		Input >> word;
	}

	return DFA(initial, finalStates, TransitionTable<Pair<State, Symbol>, State>(trans));
}
AutomatonParser::NFA AutomatonParser::ReadNFAutomaton(std::istream& Input)
{
	std::string word;
	Input >> word;

	State initial;
	LinearSet<State> finalStates;
	std::unordered_map<Pair<State, Symbol>, LinearSet<State>> trans;

	while (word.size() > 0 && Input)
	{
		if (word == "start")
		{
			Input >> initial;
		}
		else if (word == "accepts")
		{
			Input >> finalStates;
		}
		else if (word == "transitions")
		{
			State a;
			Input >> a;
			while (a.size() > 0 && Input)
			{
				State b;
				Symbol c;
				Input >> c >> b;
				trans[Pair<State, Symbol>(a, c)].Add(b);
				Input >> a;
			}
		}
		else if (word != "nfa")
		{
			throw std::invalid_argument("Word '" + word + "' was not recognized while parsing an nfa.");
		}
		Input >> word;
	}

	return NFA(initial, finalStates, TransitionTable<Pair<State, Symbol>, LinearSet<State>>(trans));
}

void AutomatonParser::Write(AutomatonParser::NFA Value, std::ostream& Output)
{
	Output << "nfa" << std::endl;
	Output << "start " << Value.getStartState() << std::endl;
	Output << "accepts" << Value.getAcceptingStates() << std::endl;
	Output << "transitions" << std::endl;
	for (auto& item : Value.getTransitionFunction().getMap())
	{
		for (auto& targetState : item.second.getItems())
		{
			Output << item.first.getFirst() << " " << item.first.getSecond() << " " << targetState << std::endl;
		}
	}
}
void AutomatonParser::Write(AutomatonParser::DFA Value, std::ostream& Output)
{
	Output << "dfa" << std::endl;
	Output << "start " << Value.getStartState() << std::endl;
	Output << "accepts" << Value.getAcceptingStates() << std::endl;
	Output << "transitions" << std::endl;
	for (auto& item : Value.getTransitionFunction().getMap())
	{
		Output << item.first.getFirst() << " " << item.first.getSecond() << " " << item.second << std::endl;
	}
}