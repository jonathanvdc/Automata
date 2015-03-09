#include "NFA.h"

#include "DFA.h"

#include <set>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

set<State> NFA::delta_hat(State q, vector<Symbol> word) {
    set<State> qs({q});
    verify_word(word);

    for (Symbol s : word) {
        set<State> new_qs;
        for (State orig : qs)
            for (State q : delta[{orig, s}])
                new_qs.insert(q);
        qs = new_qs;
    }
    return qs;
}

bool NFA::accepts(vector<Symbol> word) {
    for (State q : delta_hat(initial_state, word))
        if (is_final(q))
            return true;
    return false;
}

State fromSet(set<State> qs) {
    stringstream ss;
    ss << "{";
    bool comma = false;
    for (State q : qs) {
        if (comma) ss << ",";
        ss << q;
        comma = true;
    }
    ss << "}";
    return ss.str();  
}

DFA NFA::ssc() {
    // To prevent exponential blowup, we look for reachable states, first.
    // The initial state is always reachable...
    set<set<State>> reachable;
    reachable.insert(set<State>({initial_state}));
    
    // ...and we will also start searching from it.
    set<set<State>> step;
    step.insert(set<State>({initial_state}));
    
    // While we have NFA states left to look at:
    while (!step.empty()) {
        set<set<State>> new_step;
        // Loop over each NFA state "qs" we're looking at now.
        for (set<State> qs : step) {
            // Where will qs take us? Try each symbol s:
            for (Symbol s : alphabet) {
                // Take the union for each q in qs of delta(q, s).
                set<State> qs_after_s;
                for (State q : qs) {
                    set<State> next = delta[{q, s}];
                    for (State n : next)
                        qs_after_s.insert(n);
                }
                // This symbol might lead to a new reachable set of states.
                // If it does, we will recurse from that state next.
                bool inserted = reachable.insert(qs_after_s).second;
                if (inserted)
                    new_step.insert(qs_after_s);
            }
        }
        step = new_step;
    }
    
    // Make DFA states and initial/final states.
    State dfa_initial = fromSet(set<State>({initial_state}));
    set<State> dfa_states;
    set<State> dfa_final;
    for (set<State> s : reachable) {
        State new_state = fromSet(s);
        dfa_states.insert(new_state);
        for (State q : s) {
            if (is_final(q)) {
                dfa_final.insert(new_state);
                break;
            }
        }
    }
    
    // Make DFA transition table.
    map<pair<State, Symbol>, State> dfa_delta;
    for (set<State> s : reachable) {
        for (Symbol sym : alphabet) {
            set<State> u;
            for (State q : s)
                for (State t : delta[{q, sym}])
                    u.insert(t);
            dfa_delta[{fromSet(s), sym}] = fromSet(u);
        }
    }

    return DFA(dfa_states, alphabet, dfa_delta, dfa_initial, dfa_final);
}