#pragma once
#include <unordered_map>
#include "LinearSet.h"
#include "TransitionTable.h"
#include "Pair.h"
#include "ArraySlice.h"
#include "DFAutomaton.h"
#include "IAutomaton.h"

namespace Automata
{
    /// \brief Defines a non-deterministic finite automaton.
    template<typename TState, typename TChar>
    class NFAutomaton : public IAutomaton<TChar>
    {
    public:
        NFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<Pair<TState, TChar>, LinearSet<TState>> TransitionFunction);

        LinearSet<TState> PerformTransition(TState State, TChar Character) const;

        LinearSet<TState> PerformAllTransitions(LinearSet<TState> States, TChar Character) const;

        LinearSet<TState> PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const;

        bool ContainsAcceptingState(LinearSet<TState> States) const;

        /// \brief Gets a boolean value that indicates whether the automaton accepts the given string.
        bool Accepts(stdx::ArraySlice<TChar> Characters) const override;

        /// \brief Performs the subset construction based on the given alphabet.
        DFAutomaton<LinearSet<TState>, TChar> ToDFAutomaton(LinearSet<TChar> Alphabet) const;

        TransitionTable<Pair<TState, TChar>, LinearSet<TState>> getTransitionFunction() const;
        void setTransitionFunction(TransitionTable<Pair<TState, TChar>, LinearSet<TState>> value);

        LinearSet<TState> getAcceptingStates() const;
        void setAcceptingStates(LinearSet<TState> value);

        TState getStartState() const;
        void setStartState(TState value);
    private:
        TState StartState_value;
        LinearSet<TState> AcceptingStates_value;
        TransitionTable<Pair<TState, TChar>, LinearSet<TState>> TransitionFunction_value;
    };
}

#include "NFAutomaton.hxx"