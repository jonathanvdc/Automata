#pragma once
#include <utility>
#include <unordered_map>
#include "LinearSet.h"
#include "TransitionTable.h"
#include "Optional.h"
#include "IFunction.h"
#include "ArraySlice.h"
#include "DFAutomaton.h"
#include "IAutomaton.h"

namespace Automata
{
    /// \brief Defines a non-deterministic finite automaton with epsilon transitions.
    template<typename TState, typename TChar>
    class ENFAutomaton : public IAutomaton<TChar>
    {
    public:
        ENFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<std::pair<TState, Optional<TChar>>, LinearSet<TState>> TransitionFunction);

        LinearSet<TState> PerformTransition(TState State, Optional<TChar> Character) const;

        LinearSet<TState> PerformAllTransitions(LinearSet<TState> States, Optional<TChar> Character) const;

        LinearSet<TState> Eclose(TState State) const;

        LinearSet<TState> Eclose(LinearSet<TState> States) const;

        LinearSet<TState> PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const;

        bool ContainsAcceptingState(LinearSet<TState> States) const;

        /// \brief Gets a boolean value that indicates whether the automaton accepts the given string.
        bool Accepts(stdx::ArraySlice<TChar> Characters) const override;

        LinearSet<TState> GetStates() const;

        LinearSet<TChar> GetAlphabet() const;

        /// \brief Performs the modified subset construction on this automaton.
        DFAutomaton<LinearSet<TState>, TChar> ToDFAutomaton() const;

        TState getStartState() const;

        LinearSet<TState> getAcceptingStates() const;

        TransitionTable<std::pair<TState, Optional<TChar>>, LinearSet<TState>> getTransitionFunction() const;

		template<typename TNState, typename TNChar>
		ENFAutomaton<TNState, TNChar> Rename(const IFunction<TState, TNState>* StateRenamer, const IFunction<TChar, TNChar>* CharRenamer) const
		{
			auto newStart = StateRenamer->Apply(this->getStartState());
			LinearSet<TNState> newAccept;
			auto oldAccept = this->getAcceptingStates();
			for (auto& val : oldAccept.getItems())
			{
				newAccept.Add(StateRenamer->Apply(val));
			}
			auto currentTransFun = this->getTransitionFunction();
			std::unordered_map<std::pair<TNState, Optional<TNChar>>, LinearSet<TNState>> newTransMap;
			for (auto& item : currentTransFun.getMap())
			{
				auto renamedOriginState = StateRenamer->Apply(item.first.first);
				LinearSet<TNState> renamedTargetStates;
				for (auto& val : item.second.getItems())
				{
					renamedTargetStates.Add(StateRenamer->Apply(val));
				}
				if (item.first.second.HasValue)
				{
					newTransMap[std::pair<TNState, Optional<TNChar>>(renamedOriginState, Optional<TNChar>(CharRenamer->Apply(item.first.second.Value)))] = renamedTargetStates;
				}
				else
				{
					newTransMap[std::pair<TNState, Optional<TNChar>>(renamedOriginState, Optional<TNChar>())] = renamedTargetStates;
				}
			}
			TransitionTable<std::pair<TNState, Optional<TNChar>>, LinearSet<TNState>> transFun(newTransMap);
			return ENFAutomaton<TNState, TNChar>(newStart, newAccept, transFun);
		}
    private:
        /// \brief Performs the modified subset construction based on the given alphabet.
        DFAutomaton<LinearSet<TState>, TChar> ToDFAutomaton(LinearSet<TChar> Alphabet) const;

        void setStartState(TState value);

        void setAcceptingStates(LinearSet<TState> value);

        void setTransitionFunction(TransitionTable<std::pair<TState, Optional<TChar>>, LinearSet<TState>> value);

        TState StartState_value;
        LinearSet<TState> AcceptingStates_value;
        TransitionTable<std::pair<TState, Optional<TChar>>, LinearSet<TState>> TransitionFunction_value;
    };
}

#include "ENFAutomaton.hxx"