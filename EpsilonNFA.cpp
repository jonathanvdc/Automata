#include "EpsilonNFA.h"

#include <vector>
#include <set>
#include <queue>

using namespace std;

set<State> EpsilonNFA::eclose(State q) {
    set<State> visited;
    queue<State> todo;
    todo.push(q);

    while (!todo.empty()) {
        State p = todo.front();
        todo.pop();

        visited.insert(p);
        for (State next : delta[{p, ""}])
            if (visited.find(next) != visited.end())
                todo.push(next);
    }
    return visited;
}

set<State> EpsilonNFA::delta_hat(State q, vector<Symbol> word) {
    set<State> qs = eclose(q);
    verify_word(word);

    for (Symbol s : word) {
        set<State> new_qs;
        for (State orig : qs)
            for (State q : delta[{orig, s}])
                for (State e : eclose(q))
                    new_qs.insert(e);
        qs = new_qs;
    }
    return qs;
}

bool EpsilonNFA::accepts(vector<Symbol> word) {
    for (State q : delta_hat(initial_state, word))
        if (is_final(q))
            return true;
    return false;
}
