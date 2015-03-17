#pragma once
#include <utility>
#include <unordered_map>
#include "LinearSet.h"
#include "TransitionTable.h"
#include "ArraySlice.h"
#include "IFunction.h"
#include "IAutomaton.h"

namespace Automata
{
    /// \brief Defines a deterministic finite automaton.
    template<typename TState, typename TChar>
    class DFAutomaton : public IAutomaton<TChar>
    {
    public:
        DFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<std::pair<TState, TChar>, TState> TransitionFunction);

        TState PerformTransition(TState State, TChar Character) const;

        TState PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const;

        template<typename TNState, typename TNChar>
        DFAutomaton<TNState, TNChar> Rename(const IFunction<TState, TNState>* StateRenamer, const IFunction<TChar, TNChar>* CharRenamer) const
        {
            auto newStart = StateRenamer->Apply(this->getStartState());
            LinearSet<TNState> newAccept;
            auto oldAccept = this->getAcceptingStates();
            for (auto& item : oldAccept.getItems())
            {
                newAccept.Add(StateRenamer->Apply(item));
            }
            auto currentTransFun = this->getTransitionFunction();
            std::unordered_map<std::pair<TNState, TNChar>, TNState> newTransMap;
            for (auto& item0 : currentTransFun.getMap())
            {
                newTransMap[std::pair<TNState, TNChar>(StateRenamer->Apply(item0.first.first), CharRenamer->Apply(item0.first.second))] = StateRenamer->Apply(item0.second);
            }
            TransitionTable<std::pair<TNState, TNChar>, TNState> transFun(newTransMap);
            return DFAutomaton<TNState, TNChar>(newStart, newAccept, transFun);
        }

        LinearSet<TState> GetStates() const;

        LinearSet<TChar> GetAlphabet() const;

        bool IsAcceptingState(TState State) const;

        /// \brief Gets a boolean value that indicates whether the automaton accepts the given string.
        bool Accepts(stdx::ArraySlice<TChar> Characters) const override;

        TransitionTable<std::pair<TState, TChar>, TState> getTransitionFunction() const;

        TState getStartState() const;

        LinearSet<TState> getAcceptingStates() const;
    private:
        void setTransitionFunction(TransitionTable<std::pair<TState, TChar>, TState> value);

        void setStartState(TState value);

        void setAcceptingStates(LinearSet<TState> value);

        TState StartState_value;
        LinearSet<TState> AcceptingStates_value;
        TransitionTable<std::pair<TState, TChar>, TState> TransitionFunction_value;
    };
}

#include "DFAutomaton.hxx"