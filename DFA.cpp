#include "DFA.h"

#include <iostream>
#include <vector>

using namespace std;

State DFA::delta_hat(State q, vector<Symbol> word) {
    verify_word(word);
    for (Symbol s : word)
        q = delta[{q, s}];
    return q;
}

bool DFA::accepts(vector<Symbol> word) {
    return is_final(delta_hat(initial_state, word));
}

void DFA::print_dot() {
    // .dot header
    cout << "digraph dfa {" << endl
         << "    rankdir = LR;" << endl;

    // initial state arrow
    cout << "    __start [style = invis, shape = point];" << endl
         << "    __start -> \"" << initial_state
         << "\" [ label = \"start\" ];" << endl;

    // mark final states
    cout << "    node [shape = doublecircle];";
    for (State q : final_states)
        cout << " \"" << q << "\"";

    // draw transitions
    cout << endl << "    node [shape = circle];" << endl;
    for (auto p : delta) {
        State from = p.first.first, to = p.second;
        Symbol sym = p.first.second;
        cout << "    \"" << from << "\" -> \"" << to
             << "\" [ label = \"" << sym << "\" ];" << endl;
    }

    cout << "}" << endl;
}