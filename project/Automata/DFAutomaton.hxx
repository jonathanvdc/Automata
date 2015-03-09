#include <unordered_map>
#include <utility>
#include "LinearSet.h"
#include "TransitionTable.h"
#include "Pair.h"
#include "ArraySlice.h"
#include "IFunction.h"
#include "IAutomaton.h"
#include "DFAutomaton.h"

using namespace Automata;

template<typename TState, typename TChar>
TState DFAutomaton<TState, TChar>::PerformTransition(TState State, TChar Character) const
{
    auto transFun = this->getTransitionFunction();
    return transFun.Apply(Pair<TState, TChar>(State, Character));
}

template<typename TState, typename TChar>
TState DFAutomaton<TState, TChar>::PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const
{
    auto result = State;
    for (auto& item : Characters)
        result = this->PerformTransition(result, item);
    return result;
}


template<typename TState, typename TChar>
DFAutomaton<TState, TChar>::DFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<Pair<TState, TChar>, TState> TransitionFunction)
{
    this->setStartState(StartState);
    this->setAcceptingStates(AcceptingStates);
    this->setTransitionFunction(TransitionFunction);
}

template<typename TState, typename TChar>
bool DFAutomaton<TState, TChar>::IsAcceptingState(TState State) const
{
    auto acceptStates = this->getAcceptingStates();
    return acceptStates.Contains(State);
}

/// \brief Gets a boolean value that indicates whether the automaton accepts the given string.
template<typename TState, typename TChar>
bool DFAutomaton<TState, TChar>::Accepts(stdx::ArraySlice<TChar> Characters) const
{
    return this->IsAcceptingState(this->PerformExtendedTransition(this->getStartState(), Characters));
}

template<typename TState, typename TChar>
TransitionTable<Pair<TState, TChar>, TState> DFAutomaton<TState, TChar>::getTransitionFunction() const
{
    return this->TransitionFunction_value;
}
template<typename TState, typename TChar>
void DFAutomaton<TState, TChar>::setTransitionFunction(TransitionTable<Pair<TState, TChar>, TState> value)
{
    this->TransitionFunction_value = value;
}

template<typename TState, typename TChar>
TState DFAutomaton<TState, TChar>::getStartState() const
{
    return this->StartState_value;
}
template<typename TState, typename TChar>
void DFAutomaton<TState, TChar>::setStartState(TState value)
{
    this->StartState_value = value;
}

template<typename TState, typename TChar>
LinearSet<TState> DFAutomaton<TState, TChar>::getAcceptingStates() const
{
    return this->AcceptingStates_value;
}
template<typename TState, typename TChar>
void DFAutomaton<TState, TChar>::setAcceptingStates(LinearSet<TState> value)
{
    this->AcceptingStates_value = value;
}