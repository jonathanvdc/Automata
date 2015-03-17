#pragma once
#include <utility>
#include <unordered_map>
#include "LinearSet.h"
#include "TransitionTable.h"
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
        NFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<std::pair<TState, TChar>, LinearSet<TState>> TransitionFunction);

        LinearSet<TState> PerformTransition(TState State, TChar Character) const;

        LinearSet<TState> PerformAllTransitions(LinearSet<TState> States, TChar Character) const;

        LinearSet<TState> PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const;

        bool ContainsAcceptingState(LinearSet<TState> States) const;

        /// \brief Gets a boolean value that indicates whether the automaton accepts the given string.
        bool Accepts(stdx::ArraySlice<TChar> Characters) const override;

        LinearSet<TState> GetStates() const;

        LinearSet<TChar> GetAlphabet() const;

        /// \brief Performs the subset construction on this automaton.
        DFAutomaton<LinearSet<TState>, TChar> ToDFAutomaton() const;

        TransitionTable<std::pair<TState, TChar>, LinearSet<TState>> getTransitionFunction() const;

        LinearSet<TState> getAcceptingStates() const;

        TState getStartState() const;
    private:
        /// \brief Performs the subset construction based on the given alphabet.
        DFAutomaton<LinearSet<TState>, TChar> ToDFAutomaton(LinearSet<TChar> Alphabet) const;

        void setTransitionFunction(TransitionTable<std::pair<TState, TChar>, LinearSet<TState>> value);

        void setAcceptingStates(LinearSet<TState> value);

        void setStartState(TState value);

        TState StartState_value;
        LinearSet<TState> AcceptingStates_value;
        TransitionTable<std::pair<TState, TChar>, LinearSet<TState>> TransitionFunction_value;
    };
}

#include "NFAutomaton.hxx"