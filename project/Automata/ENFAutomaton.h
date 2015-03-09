#pragma once
#include "LinearSet.h"
#include "TransitionTable.h"
#include "Pair.h"
#include "Optional.h"
#include "ArraySlice.h"
#include "IAutomaton.h"

namespace Automata
{
    /// \brief Defines a non-deterministic finite automaton with epsilon transitions.
    template<typename TState, typename TChar>
    class ENFAutomaton : public IAutomaton<TChar>
    {
    public:
        ENFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<Pair<TState, Optional<TChar>>, LinearSet<TState>> TransitionFunction);

        LinearSet<TState> PerformTransition(TState State, Optional<TChar> Character) const;

        LinearSet<TState> PerformAllTransitions(LinearSet<TState> States, Optional<TChar> Character) const;

        LinearSet<TState> Eclose(TState State) const;

        LinearSet<TState> Eclose(LinearSet<TState> States) const;

        LinearSet<TState> PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const;

        bool ContainsAcceptingState(LinearSet<TState> States) const;

        /// \brief Gets a boolean value that indicates whether the automaton accepts the given string.
        bool Accepts(stdx::ArraySlice<TChar> Characters) const override;

        TransitionTable<Pair<TState, Optional<TChar>>, LinearSet<TState>> getTransitionFunction() const;
        void setTransitionFunction(TransitionTable<Pair<TState, Optional<TChar>>, LinearSet<TState>> value);

        LinearSet<TState> getAcceptingStates() const;
        void setAcceptingStates(LinearSet<TState> value);

        TState getStartState() const;
        void setStartState(TState value);
    private:
        TState StartState_value;
        LinearSet<TState> AcceptingStates_value;
        TransitionTable<Pair<TState, Optional<TChar>>, LinearSet<TState>> TransitionFunction_value;
    };
}

#include "ENFAutomaton.hxx"