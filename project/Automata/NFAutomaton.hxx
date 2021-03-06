#include "NFAutomaton.h"

#include <unordered_map>
#include <utility>
#include "ArraySlice.h"
#include "DFAutomaton.h"
#include "IAutomaton.h"
#include "LinearSet.h"
#include "TransitionTable.h"

using namespace Automata;

/// \brief Creates a non-deterministic finite automaton based on the
/// given start state, set of accepting states, and transition
/// function.
template<typename TState, typename TChar>
NFAutomaton<TState, TChar>::NFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<std::pair<TState, TChar>, LinearSet<TState>> TransitionFunction)
{
    this->setStartState(StartState);
    this->setAcceptingStates(AcceptingStates);
    this->setTransitionFunction(TransitionFunction);
}

/// \brief Figures out whether this automaton accepts the given
/// string of symbols.
template<typename TState, typename TChar>
bool NFAutomaton<TState, TChar>::Accepts(stdx::ArraySlice<TChar> Characters) const
{
    return this->ContainsAcceptingState(this->PerformExtendedTransition(this->getStartState(), 
                                                                        Characters));
}

/// \brief Finds out whether the given sequence of states has a
/// non-empty intersection with the set of accepting states.
template<typename TState, typename TChar>
bool NFAutomaton<TState, TChar>::ContainsAcceptingState(LinearSet<TState> States) const
{
    auto acceptStates = this->getAcceptingStates();
    auto intersection = acceptStates.Intersect(States);
    return !intersection.getIsEmpty();
}

/// \brief Gets the automaton's alphabet.
template<typename TState, typename TChar>
LinearSet<TChar> NFAutomaton<TState, TChar>::GetAlphabet() const
{
    LinearSet<TChar> results;
    auto transfunc = this->getTransitionFunction();
    for (auto& item : transfunc.getMap())
        results.Add(item.first.second);
    return results;
}

/// \brief Gets the set of all states in this automaton.
template<typename TState, typename TChar>
LinearSet<TState> NFAutomaton<TState, TChar>::GetStates() const
{
    LinearSet<TState> results;
    auto transfunc = this->getTransitionFunction();
    for (auto& item : transfunc.getMap())
    {
        results.Add(item.first.first);
        results.AddAll(item.second);
    }
    return results;
}

/// \brief Returns the set of all states reachable by performing a
/// single transition with the given character for each state
/// in the given state set.
template<typename TState, typename TChar>
LinearSet<TState> NFAutomaton<TState, TChar>::PerformAllTransitions(LinearSet<TState> States, TChar Character) const
{
    LinearSet<TState> results;
    for (auto& val : States.getItems())
        results.AddAll(this->PerformTransition(val, Character));
    return results;
}

/// \brief Applies the automaton's extended transition function to
/// the given state and string of symbols.
template<typename TState, typename TChar>
LinearSet<TState> NFAutomaton<TState, TChar>::PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const
{
    LinearSet<TState> states;
    states.Add(State);
    for (auto& item : Characters)
        states = this->PerformAllTransitions(states, item);
    return states;
}

/// \brief Applies the automaton's transition function to the given
/// state and symbol.
template<typename TState, typename TChar>
LinearSet<TState> NFAutomaton<TState, TChar>::PerformTransition(TState State, TChar Character) const
{
    auto transFun = this->getTransitionFunction();
    return transFun.Apply(std::pair<TState, TChar>(State, Character));
}

/// \brief Performs the subset construction on this automaton.
template<typename TState, typename TChar>
DFAutomaton<LinearSet<TState>, TChar> NFAutomaton<TState, TChar>::ToDFAutomaton() const
{
    return this->ToDFAutomaton(this->GetAlphabet());
}

/// \brief Performs the subset construction based on the given
/// alphabet.
template<typename TState, typename TChar>
DFAutomaton<LinearSet<TState>, TChar> NFAutomaton<TState, TChar>::ToDFAutomaton(LinearSet<TChar> Alphabet) const
{
    LinearSet<TState> startState;
    startState.Add(this->getStartState());
    std::unordered_map<std::pair<LinearSet<TState>, TChar>, LinearSet<TState>> transMap;
    LinearSet<LinearSet<TState>> accStates;
    LinearSet<LinearSet<TState>> processedStates;
    LinearSet<LinearSet<TState>> nextStates;
    LinearSet<TState> initialState;
    initialState.Add(this->getStartState());
    nextStates.Add(initialState);
    while (!nextStates.getIsEmpty())
    {
        auto last = nextStates.getLast();
        LinearSet<LinearSet<TState>> accumulatedStates;
        if (!processedStates.Contains(last))
        {
            for (auto& item : Alphabet.getItems())
            {
                auto trans = this->PerformAllTransitions(last, item);
                accumulatedStates.Add(trans);
                transMap[std::pair<LinearSet<TState>, TChar>(last, item)] = trans;
            }
            processedStates.Add(last);
            if (this->ContainsAcceptingState(last))
                accStates.Add(last);

        }
        nextStates.RemoveLast();
        nextStates.AddAll(accumulatedStates);
    }
    TransitionTable<std::pair<LinearSet<TState>, TChar>, LinearSet<TState>> transFun(transMap);
    return DFAutomaton<LinearSet<TState>, TChar>(startState, 
                                                 accStates, transFun);
}

/// \brief Gets this automaton's set of accepting states.
template<typename TState, typename TChar>
LinearSet<TState> NFAutomaton<TState, TChar>::getAcceptingStates() const
{
    return this->AcceptingStates_value;
}

/// \brief Sets this automaton's set of accepting states.
template<typename TState, typename TChar>
void NFAutomaton<TState, TChar>::setAcceptingStates(LinearSet<TState> value)
{
    this->AcceptingStates_value = value;
}

/// \brief Gets this automaton's start state.
template<typename TState, typename TChar>
TState NFAutomaton<TState, TChar>::getStartState() const
{
    return this->StartState_value;
}

/// \brief Sets this automaton's start state.
template<typename TState, typename TChar>
void NFAutomaton<TState, TChar>::setStartState(TState value)
{
    this->StartState_value = value;
}

/// \brief Gets this automaton's transition function.
template<typename TState, typename TChar>
TransitionTable<std::pair<TState, TChar>, LinearSet<TState>> NFAutomaton<TState, TChar>::getTransitionFunction() const
{
    return this->TransitionFunction_value;
}

/// \brief Sets this automaton's transition function.
template<typename TState, typename TChar>
void NFAutomaton<TState, TChar>::setTransitionFunction(TransitionTable<std::pair<TState, TChar>, LinearSet<TState>> value)
{
    this->TransitionFunction_value = value;
}