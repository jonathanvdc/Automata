#pragma once
#include <unordered_map>
#include <utility>
#include "ArraySlice.h"
#include "IAutomaton.h"
#include "IFunction.h"
#include "LinearSet.h"
#include "TransitionTable.h"

namespace Automata
{
    /// \brief Defines a deterministic finite automaton.
    template<typename TState, typename TChar>
    class DFAutomaton : public virtual IAutomaton<TChar>
    {
    public:
        /// \brief Creates a deterministic finite automaton based on the
        /// given start state, set of accepting states, and transition
        /// function.
        DFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<std::pair<TState, TChar>, TState> TransitionFunction);

        /// \brief Checks if this automaton accepts the given string of
        /// symbols.
        bool Accepts(stdx::ArraySlice<TChar> Characters) const override;

        /// \brief Checks if this automaton is equivalent to the given other
        /// automaton.
        bool EquivalentTo(DFAutomaton<TState, TChar> other) const;

        /// \brief Gets this automaton's set of accepting states.
        LinearSet<TState> getAcceptingStates() const;

        /// \brief Gets this automaton's alphabet, as a set of characters.
        LinearSet<TChar> GetAlphabet() const;

        /// \brief Gets this automaton's start state.
        TState getStartState() const;

        /// \brief Gets the set of all states in this automaton.
        LinearSet<TState> GetStates() const;

        /// \brief Gets this automaton's transition function.
        TransitionTable<std::pair<TState, TChar>, TState> getTransitionFunction() const;

        /// \brief Finds out if the given state is an accepting state.
        bool IsAcceptingState(TState State) const;

        /// \brief Optimizes this automaton. First, all reachable states are
        /// detected. Then, the TFA is applied to these reachable
        /// states, and all equivalent states are merged.
        /// A new automaton is constructed based on these merged
        /// states, and then returned.
        DFAutomaton<LinearSet<TState>, TChar> Optimize() const;

        /// \brief Applies the automaton's extended transition function to
        /// the given state and string of symbols.
        TState PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const;

        /// \brief Applies the automaton's transition function to the given
        /// state and symbol.
        TState PerformTransition(TState State, TChar Character) const;

        /// \brief Gets the set of all reachable states in this automaton.
        LinearSet<TState> ReachableStates() const;

        /// \brief Renames this automaton with the given state and character
        /// renamer.
        template<typename TNState, typename TNChar>
        DFAutomaton<TNState, TNChar> Rename(const IFunction<TState, TNState>* StateRenamer, const IFunction<TChar, TNChar>* CharRenamer) const
        {
            auto newStart = StateRenamer->Apply(this->getStartState());
            LinearSet<TNState> newAccept;
            auto oldAccept = this->getAcceptingStates();
            for (auto& item : oldAccept.getItems())
                newAccept.Add(StateRenamer->Apply(item));
            auto currentTransFun = this->getTransitionFunction();
            std::unordered_map<std::pair<TNState, TNChar>, TNState> newTransMap;
            for (auto& item0 : currentTransFun.getMap())
                newTransMap[std::pair<TNState, TNChar>(StateRenamer->Apply(item0.first.first), 
                                                       CharRenamer->Apply(item0.first.second))] = StateRenamer->Apply(item0.second);
            TransitionTable<std::pair<TNState, TNChar>, TNState> transFun(newTransMap);
            return DFAutomaton<TNState, TNChar>(newStart, newAccept, transFun);
        }

        /// \brief Performs the table-filling algorithm on this automaton,
        /// and returns the resulting partition of equivalent states.
        std::unordered_map<TState, LinearSet<TState>> TFAPartition() const;
    private:
        /// \brief Sets this automaton's set of accepting states.
        void setAcceptingStates(LinearSet<TState> value);

        /// \brief Sets this automaton's start state.
        void setStartState(TState value);

        /// \brief Sets this automaton's transition function.
        void setTransitionFunction(TransitionTable<std::pair<TState, TChar>, TState> value);

        LinearSet<TState> AcceptingStates_value;
        TState StartState_value;
        TransitionTable<std::pair<TState, TChar>, TState> TransitionFunction_value;
    };
}

#include "DFAutomaton.hxx"