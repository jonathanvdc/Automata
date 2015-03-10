#pragma once
#include <memory>
#include <iostream>
#include "NFAutomaton.h"
#include "DFAutomaton.h"
#include "ENFAutomaton.h"

namespace Automata
{
	struct AutomatonParser
	{
		typedef std::string State;
		typedef std::string Symbol;
		typedef DFAutomaton<State, Symbol> DFA;
		typedef NFAutomaton<State, Symbol> NFA;
		typedef ENFAutomaton<State, Symbol> ENFA;
		typedef std::unique_ptr<IAutomaton<Symbol>> Automaton;

		Automaton ReadAutomaton(std::istream& Input);
		DFA ReadDFAutomaton(std::istream& Input);
		NFA ReadNFAutomaton(std::istream& Input);
		ENFA ReadENFAutomaton(std::istream& Input);

		void Write(NFA Value, std::ostream& Output);
		void Write(DFA Value, std::ostream& Output);
		void Write(ENFA Value, std::ostream& Output);
	};
}