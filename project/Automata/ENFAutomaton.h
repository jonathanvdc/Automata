#pragma once
#include <utility>
#include "ArraySlice.h"
#include "DFAutomaton.h"
#include "IAutomaton.h"
#include "IFunction.h"
#include "LinearSet.h"
#include "Optional.h"
#include "TransitionTable.h"

namespace Automata
{
    /// \brief Defines a non-deterministic finite automaton with epsilon
    /// transitions.
    template<typename TState, typename TChar>
    class ENFAutomaton : public virtual IAutomaton<TChar>
    {
    public:
        /// \brief Creates a non-deterministic finite automaton with epsilon
        /// transitions from the given start state, accepting states
        /// and transition functions.
        ENFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<std::pair<TState, Optional<TChar>>, LinearSet<TState>> TransitionFunction);

        /// \brief Figures out whether this automaton accepts the given
        /// string of symbols.
        bool Accepts(stdx::ArraySlice<TChar> Characters) const override;

        /// \brief Finds out whether the given sequence of states has a
        /// non-empty intersection with the set of accepting states.
        bool ContainsAcceptingState(LinearSet<TState> States) const;

        /// \brief Computes the epsilon-closure of the given state.
        LinearSet<TState> Eclose(TState State) const;

        /// \brief Computes the epsilon-closure of the given set of states.
        LinearSet<TState> Eclose(LinearSet<TState> States) const;

        /// \brief Gets the e-NFA's set of accepting states.
        LinearSet<TState> getAcceptingStates() const;

        /// \brief Gets the automaton's alphabet.
        LinearSet<TChar> GetAlphabet() const;

        /// \brief Gets the e-NFA's start state.
        TState getStartState() const;

        /// \brief Gets the set of all states in this automaton.
        LinearSet<TState> GetStates() const;

        /// \brief Gets the e-NFA's transition function.
        TransitionTable<std::pair<TState, Optional<TChar>>, LinearSet<TState>> getTransitionFunction() const;

        /// \brief Returns the set of all states reachable by performing a
        /// single transition with the given character for each state
        /// in the given state set.
        LinearSet<TState> PerformAllTransitions(LinearSet<TState> States, Optional<TChar> Character) const;

        /// \brief Performs the extended transition that starts at the given
        /// state and persues the labels that match the elements of the
        /// given sequence of characters, also performing the
        /// epsilon-closure.
        LinearSet<TState> PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const;

        /// \brief Performs a state transition that starts at the given
        /// state, and uses the given character (or epsilon, if none is
        /// given).
        LinearSet<TState> PerformTransition(TState State, Optional<TChar> Character) const;

        /// \brief Creates a new e-NFA by renaming this e-NFA's states and
        /// symbols.
        template<typename TNState, typename TNChar>
        ENFAutomaton<TNState, TNChar> Rename(const IFunction<TState, TNState>* StateRenamer, const IFunction<TChar, TNChar>* CharRenamer) const
        {
            auto newStart = StateRenamer->Apply(this->getStartState());
            LinearSet<TNState> newAccept;
            auto oldAccept = this->getAcceptingStates();
            for (auto& val : oldAccept.getItems())
                newAccept.Add(StateRenamer->Apply(val));
            auto currentTransFun = this->getTransitionFunction();
            std::unordered_map<std::pair<TNState, Optional<TNChar>>, LinearSet<TNState>> newTransMap;
            for (auto& item : currentTransFun.getMap())
            {
                auto renamedOriginState = StateRenamer->Apply(item.first.first);
                LinearSet<TNState> renamedTargetStates;
                for (auto& state : item.second.getItems())
                    renamedTargetStates.Add(StateRenamer->Apply(state));
                if (item.first.second.HasValue)
                    newTransMap[std::pair<TNState, Optional<TNChar>>(renamedOriginState, 
                                                         Optional<TNChar>(CharRenamer->Apply(item.first.second.Value)))] = renamedTargetStates;
                else
                    newTransMap[std::pair<TNState, Optional<TNChar>>(renamedOriginState, 
                                                         Optional<TNChar>())] = renamedTargetStates;

            }
            TransitionTable<std::pair<TNState, Optional<TNChar>>, LinearSet<TNState>> transFun(newTransMap);
            return ENFAutomaton<TNState, TNChar>(newStart, newAccept, transFun);
        }

        /// \brief Performs the modified subset construction on this
        /// automaton.
        DFAutomaton<LinearSet<TState>, TChar> ToDFAutomaton() const;
    private:
        /// \brief Sets the e-NFA's set of accepting states.
        void setAcceptingStates(LinearSet<TState> value);

        /// \brief Sets the e-NFA's start state.
        void setStartState(TState value);

        /// \brief Sets the e-NFA's transition function.
        void setTransitionFunction(TransitionTable<std::pair<TState, Optional<TChar>>, LinearSet<TState>> value);

        /// \brief Performs the modified subset construction based on the
        /// given alphabet.
        DFAutomaton<LinearSet<TState>, TChar> ToDFAutomaton(LinearSet<TChar> Alphabet) const;

        LinearSet<TState> AcceptingStates_value;
        TState StartState_value;
        TransitionTable<std::pair<TState, Optional<TChar>>, LinearSet<TState>> TransitionFunction_value;
    };
}

#include "ENFAutomaton.hxx"