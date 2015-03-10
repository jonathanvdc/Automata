#include "AutomatonDotPrinter.h"
#include <memory>
#include <iostream>
#include <string>
#include "DFAutomaton.h"
#include "HashExtensions.h"

void AutomatonDotPrinter::Write(AutomatonDotPrinter::DFA Value, std::ostream& Output)
{
	// .dot header 
	Output << "digraph dfa {" << std::endl
		<< "    rankdir = LR;" << std::endl;

	// initial state arrow 
	Output << "    __start [style = invis, shape = point];" << std::endl
		<< "    __start -> \"" << Value.getStartState()
		<< "\" [ label = \"start\" ];" << std::endl;

	// mark final states 
	Output << "    node [shape = doublecircle];";
	for (State q : Value.getAcceptingStates().getItems())
		Output << " \"" << q << "\"";

	// draw transitions 
	Output << std::endl << "    node [shape = circle];" << std::endl;
	for (auto p : Value.getTransitionFunction().getMap()) {
		State from = p.first.first, to = p.second;
		Symbol sym = p.first.second;
		Output << "    \"" << from << "\" -> \"" << to
			<< "\" [ label = \"" << sym << "\" ];" << std::endl;

	}

	Output << "}" << std::endl;
}
