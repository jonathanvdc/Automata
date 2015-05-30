#pragma once
#include <utility>
#include "ArraySlice.h"
#include "DFAutomaton.h"
#include "IAutomaton.h"
#include "LinearSet.h"
#include "TransitionTable.h"

namespace Automata
{
    /// \brief Defines a non-deterministic finite automaton.
    template<typename TState, typename TChar>
    class NFAutomaton : public virtual IAutomaton<TChar>
    {
    public:
        /// \brief Creates a non-deterministic finite automaton based on the
        /// given start state, set of accepting states, and transition
        /// function.
        NFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<std::pair<TState, TChar>, LinearSet<TState>> TransitionFunction);

        /// \brief Figures out whether this automaton accepts the given
        /// string of symbols.
        bool Accepts(stdx::ArraySlice<TChar> Characters) const override;

        /// \brief Finds out whether the given sequence of states has a
        /// non-empty intersection with the set of accepting states.
        bool ContainsAcceptingState(LinearSet<TState> States) const;

        /// \brief Gets this automaton's set of accepting states.
        LinearSet<TState> getAcceptingStates() const;

        /// \brief Gets the automaton's alphabet.
        LinearSet<TChar> GetAlphabet() const;

        /// \brief Gets this automaton's start state.
        TState getStartState() const;

        /// \brief Gets the set of all states in this automaton.
        LinearSet<TState> GetStates() const;

        /// \brief Gets this automaton's transition function.
        TransitionTable<std::pair<TState, TChar>, LinearSet<TState>> getTransitionFunction() const;

        /// \brief Returns the set of all states reachable by performing a
        /// single transition with the given character for each state
        /// in the given state set.
        LinearSet<TState> PerformAllTransitions(LinearSet<TState> States, TChar Character) const;

        /// \brief Applies the automaton's extended transition function to
        /// the given state and string of symbols.
        LinearSet<TState> PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const;

        /// \brief Applies the automaton's transition function to the given
        /// state and symbol.
        LinearSet<TState> PerformTransition(TState State, TChar Character) const;

        /// \brief Performs the subset construction on this automaton.
        DFAutomaton<LinearSet<TState>, TChar> ToDFAutomaton() const;
    private:
        /// \brief Sets this automaton's set of accepting states.
        void setAcceptingStates(LinearSet<TState> value);

        /// \brief Sets this automaton's start state.
        void setStartState(TState value);

        /// \brief Sets this automaton's transition function.
        void setTransitionFunction(TransitionTable<std::pair<TState, TChar>, LinearSet<TState>> value);

        /// \brief Performs the subset construction based on the given
        /// alphabet.
        DFAutomaton<LinearSet<TState>, TChar> ToDFAutomaton(LinearSet<TChar> Alphabet) const;

        LinearSet<TState> AcceptingStates_value;
        TState StartState_value;
        TransitionTable<std::pair<TState, TChar>, LinearSet<TState>> TransitionFunction_value;
    };
}

#include "NFAutomaton.hxx"