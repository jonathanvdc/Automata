#pragma once
#include <unordered_map>
#include <utility>
#include "LinearSet.h"
#include "TransitionTable.h"
#include "Pair.h"
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
        DFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<Pair<TState, TChar>, TState> TransitionFunction);

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
            std::unordered_map<Pair<TNState, TNChar>, TNState> newTransMap;
            for (auto& item0 : currentTransFun.getMap())
            {
                newTransMap[Pair<TNState, TNChar>(StateRenamer->Apply(item0.first.getFirst()), CharRenamer->Apply(item0.first.getSecond()))] = StateRenamer->Apply(item0.second);
            }
            TransitionTable<Pair<TNState, TNChar>, TNState> transFun(newTransMap);
            return DFAutomaton<TNState, TNChar>(newStart, newAccept, transFun);
        }

        bool IsAcceptingState(TState State) const;

        /// \brief Gets a boolean value that indicates whether the automaton accepts the given string.
        bool Accepts(stdx::ArraySlice<TChar> Characters) const override;

        TransitionTable<Pair<TState, TChar>, TState> getTransitionFunction() const;
        void setTransitionFunction(TransitionTable<Pair<TState, TChar>, TState> value);

        TState getStartState() const;
        void setStartState(TState value);

        LinearSet<TState> getAcceptingStates() const;
        void setAcceptingStates(LinearSet<TState> value);
    private:
        TState StartState_value;
        LinearSet<TState> AcceptingStates_value;
        TransitionTable<Pair<TState, TChar>, TState> TransitionFunction_value;
    };
}

#include "DFAutomaton.hxx"