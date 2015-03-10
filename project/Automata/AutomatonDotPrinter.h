#pragma once
#include <memory>
#include <iostream>
#include <string>
#include "DFAutomaton.h"

namespace Automata
{
	struct AutomatonDotPrinter
	{
		typedef std::string State;
		typedef std::string Symbol;
		typedef DFAutomaton<State, Symbol> DFA;
		typedef std::unique_ptr<IAutomaton<Symbol>> Automaton;

		void Write(DFA Value, std::ostream& Output);
	};
}