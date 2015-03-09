#pragma once

#include "BaseAutomaton.h"
#include "DFA.h"

#include <map>
#include <set>
#include <vector>
#include <utility>

using namespace std;

class NFA : public BaseAutomaton {
    typedef map<pair<State, Symbol>, set<State>> TransitionMap;

public:
    NFA(set<State> q, set<Symbol> s, TransitionMap d, State q0,
            set<State> f) : BaseAutomaton(q, s, q0, f), delta(d) {
        for (auto const &p : d) {
            State from_state = p.first.first;
            assert(q.find(from_state) != q.end());
            Symbol sym = p.first.second;
            assert(s.find(sym) != s.end());
            for (State to_state : p.second)
                assert(q.find(to_state) != q.end());
        }
    }

    TransitionMap delta;
    set<State> delta_hat(State q, vector<Symbol> word);
    bool accepts(vector<Symbol> word);
    DFA ssc();
};
