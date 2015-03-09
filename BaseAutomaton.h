#pragma once

#include <cassert>
#include <map>
#include <set>
#include <vector>
#include <utility>

using namespace std;

typedef string State;
typedef string Symbol;

class BaseAutomaton {
public:
    BaseAutomaton(set<State> q, set<Symbol> s, State q0, set<State> f)
        : states(q), alphabet(s), initial_state(q0), final_states(f) {
        assert(q.find(q0) != q.end());
        for (State p : f)
            assert(q.find(p) != q.end());
    }

    set<State> states;
    set<Symbol> alphabet;
    State initial_state;
    set<State> final_states;

    bool is_final(State q) {
        return final_states.find(q) != final_states.end();
    }

    void verify_word(vector<Symbol> word) {
        for (Symbol sym : word)
            assert(alphabet.find(sym) != alphabet.end());
    }

    // Pure virtual function -- a deriving class *must* implement this.
    virtual bool accepts(vector<Symbol> word) = 0;
};
