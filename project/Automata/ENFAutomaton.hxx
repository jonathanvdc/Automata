#include "LinearSet.h"
#include "TransitionTable.h"
#include "Pair.h"
#include "Optional.h"
#include "ArraySlice.h"
#include "IAutomaton.h"
#include "ENFAutomaton.h"

using namespace Automata;

template<typename TState, typename TChar>
LinearSet<TState> ENFAutomaton<TState, TChar>::PerformTransition(TState State, Optional<TChar> Character) const
{
    auto transFun = this->getTransitionFunction();
    return transFun.Apply(Pair<TState, Optional<TChar>>(State, Character));
}

template<typename TState, typename TChar>
LinearSet<TState> ENFAutomaton<TState, TChar>::PerformAllTransitions(LinearSet<TState> States, Optional<TChar> Character) const
{
    LinearSet<TState> results;
    for (auto& val : States.getItems())
    {
        results.AddAll(this->PerformTransition(val, Character));
    }
    return results;
}

template<typename TState, typename TChar>
LinearSet<TState> ENFAutomaton<TState, TChar>::Eclose(TState State) const
{
    LinearSet<TState> results;
    LinearSet<TState> step;
    step.Add(State);
    while (!step.getIsEmpty())
    {
        auto last = step.getLast();
        results.Add(last);
        step.RemoveLast();
        LinearSet<TState> nextStep;
        auto trans = this->PerformTransition(last, Optional<TChar>());
        for (auto& item : trans.getItems())
        {
            if (!results.Contains(item))
            {
                step.Add(item);
            }
        }
    }
    return results;
}

template<typename TState, typename TChar>
LinearSet<TState> ENFAutomaton<TState, TChar>::Eclose(LinearSet<TState> States) const
{
    LinearSet<TState> vals;
    for (auto& item : States.getItems())
    {
        vals.AddAll(this->Eclose(item));
    }
    return vals;
}

template<typename TState, typename TChar>
LinearSet<TState> ENFAutomaton<TState, TChar>::PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const
{
    auto states = this->Eclose(State);
    for (auto& item : Characters)
    {
        Optional<TChar> optItem(item);
        states = this->Eclose(this->PerformAllTransitions(states, optItem));
    }
    return states;
}

template<typename TState, typename TChar>
bool ENFAutomaton<TState, TChar>::ContainsAcceptingState(LinearSet<TState> States) const
{
    auto acceptStates = this->getAcceptingStates();
    auto intersection = acceptStates.Intersect(States);
    return !intersection.getIsEmpty();
}

/// \brief Gets a boolean value that indicates whether the automaton accepts the given string.
template<typename TState, typename TChar>
bool ENFAutomaton<TState, TChar>::Accepts(stdx::ArraySlice<TChar> Characters) const
{
    return this->ContainsAcceptingState(this->PerformExtendedTransition(this->getStartState(), Characters));
}

template<typename TState, typename TChar>
ENFAutomaton<TState, TChar>::ENFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<Pair<TState, Optional<TChar>>, LinearSet<TState>> TransitionFunction)
{
    this->setStartState(StartState);
    this->setAcceptingStates(AcceptingStates);
    this->setTransitionFunction(TransitionFunction);
}

template<typename TState, typename TChar>
TransitionTable<Pair<TState, Optional<TChar>>, LinearSet<TState>> ENFAutomaton<TState, TChar>::getTransitionFunction() const
{
    return this->TransitionFunction_value;
}
template<typename TState, typename TChar>
void ENFAutomaton<TState, TChar>::setTransitionFunction(TransitionTable<Pair<TState, Optional<TChar>>, LinearSet<TState>> value)
{
    this->TransitionFunction_value = value;
}

template<typename TState, typename TChar>
LinearSet<TState> ENFAutomaton<TState, TChar>::getAcceptingStates() const
{
    return this->AcceptingStates_value;
}
template<typename TState, typename TChar>
void ENFAutomaton<TState, TChar>::setAcceptingStates(LinearSet<TState> value)
{
    this->AcceptingStates_value = value;
}

template<typename TState, typename TChar>
TState ENFAutomaton<TState, TChar>::getStartState() const
{
    return this->StartState_value;
}
template<typename TState, typename TChar>
void ENFAutomaton<TState, TChar>::setStartState(TState value)
{
    this->StartState_value = value;
}