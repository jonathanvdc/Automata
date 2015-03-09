#pragma once

#include "BaseAutomaton.h"

#include <cassert>
#include <map>
#include <set>
#include <vector>
#include <utility>

using namespace std;

class DFA : public BaseAutomaton {
    typedef map<pair<State, Symbol>, State> TransitionMap;

public:
    DFA(set<State> q, set<Symbol> s, TransitionMap d, State q0,
            set<State> f) : BaseAutomaton(q, s, q0, f), delta(d) {
        for (auto const &p : d) {
            State from_state = p.first.first;
            assert(q.find(from_state) != q.end());
            Symbol sym = p.first.second;
            assert(s.find(sym) != s.end());
            State to_state = p.second;
            assert(q.find(to_state) != q.end());
        }
    }

    TransitionMap delta;
    State delta_hat(State q, vector<Symbol> word);
    bool accepts(vector<Symbol> word);
    
    void print_dot();
};
