#include "DFAutomaton.h"

#include <unordered_map>
#include <utility>
#include "ArraySlice.h"
#include "IAutomaton.h"
#include "IdFunction.h"
#include "IFunction.h"
#include "LinearSet.h"
#include "TagFunction.h"
#include "TransitionTable.h"

using namespace Automata;

/// \brief Creates a deterministic finite automaton based on the
/// given start state, set of accepting states, and transition
/// function.
template<typename TState, typename TChar>
DFAutomaton<TState, TChar>::DFAutomaton(TState StartState, LinearSet<TState> AcceptingStates, TransitionTable<std::pair<TState, TChar>, TState> TransitionFunction)
{
    this->setStartState(StartState);
    this->setAcceptingStates(AcceptingStates);
    this->setTransitionFunction(TransitionFunction);
}

/// \brief Checks if this automaton accepts the given string of
/// symbols.
template<typename TState, typename TChar>
bool DFAutomaton<TState, TChar>::Accepts(stdx::ArraySlice<TChar> Characters) const
{
    return this->IsAcceptingState(this->PerformExtendedTransition(this->getStartState(), 
                                                                  Characters));
}

/// \brief Checks if this automaton is equivalent to the given other
/// automaton.
template<typename TState, typename TChar>
bool DFAutomaton<TState, TChar>::EquivalentTo(DFAutomaton<TState, TChar> other) const
{
    TagFunction<TState, bool> tagFalse(false);
    TagFunction<TState, bool> tagTrue(true);
    IdFunction<TChar> idFunc;
    auto taggedThis = this->Rename<std::pair<TState, bool>, TChar>(&tagFalse, 
                                                                   &idFunc);
    auto taggedOther = other.Rename<std::pair<TState, bool>, TChar>(&tagTrue, 
                                                                    &idFunc);
    auto mergedTable = taggedThis.getTransitionFunction();
    mergedTable.Add(taggedOther.getTransitionFunction());
    auto mergedAcceptingStates = taggedThis.getAcceptingStates();
    mergedAcceptingStates.AddAll(taggedOther.getAcceptingStates());
    DFAutomaton<std::pair<TState, bool>, TChar> mergedAutomaton(taggedThis.getStartState(), 
                                                                mergedAcceptingStates, 
                                                                mergedTable);
    auto partition = mergedAutomaton.TFAPartition();
    return partition[taggedThis.getStartState()] == 
           partition[taggedOther.getStartState()];
}

/// \brief Gets this automaton's alphabet, as a set of characters.
template<typename TState, typename TChar>
LinearSet<TChar> DFAutomaton<TState, TChar>::GetAlphabet() const
{
    LinearSet<TChar> results;
    auto transfunc = this->getTransitionFunction();
    for (auto& item : transfunc.getMap())
        results.Add(item.first.second);
    return results;
}

/// \brief Gets the set of all states in this automaton.
template<typename TState, typename TChar>
LinearSet<TState> DFAutomaton<TState, TChar>::GetStates() const
{
    LinearSet<TState> results;
    auto transfunc = this->getTransitionFunction();
    for (auto& item : transfunc.getMap())
    {
        results.Add(item.first.first);
        results.Add(item.second);
    }
    return results;
}

/// \brief Finds out if the given state is an accepting state.
template<typename TState, typename TChar>
bool DFAutomaton<TState, TChar>::IsAcceptingState(TState State) const
{
    auto acceptStates = this->getAcceptingStates();
    return acceptStates.Contains(State);
}

/// \brief Optimizes this automaton. First, all reachable states are
/// detected. Then, the TFA is applied to these reachable
/// states, and all equivalent states are merged.
/// A new automaton is constructed based on these merged
/// states, and then returned.
template<typename TState, typename TChar>
DFAutomaton<LinearSet<TState>, TChar> DFAutomaton<TState, TChar>::Optimize() const
{
    TState sourceState;
    auto reachable = this->ReachableStates();
    TransitionTable<std::pair<TState, TChar>, TState> filteredTable;
    auto oldTable = this->getTransitionFunction();
    for (auto& item : oldTable.getMap())
    {
        sourceState = item.first.first;
        if (reachable.Contains(sourceState))
            filteredTable.Add(item.first, item.second);

    }
    auto partition = this->TFAPartition();
    TransitionTable<std::pair<LinearSet<TState>, TChar>, LinearSet<TState>> optimalTable;
    for (auto& item0 : filteredTable.getMap())
    {
        sourceState = item0.first.first;
        auto symbol = item0.first.second;
        auto targetState = item0.second;
        std::pair<LinearSet<TState>, TChar> newPair(partition[sourceState], 
                                                    symbol);
        optimalTable.Add(newPair, partition[targetState]);
    }
    LinearSet<LinearSet<TState>> final;
    auto accStates = this->getAcceptingStates();
    for (auto& q : accStates.getItems())
        final.Add(partition[q]);
    return DFAutomaton<LinearSet<TState>, TChar>(partition[this->getStartState()], 
                                                 final, optimalTable);
}

/// \brief Applies the automaton's extended transition function to
/// the given state and string of symbols.
template<typename TState, typename TChar>
TState DFAutomaton<TState, TChar>::PerformExtendedTransition(TState State, stdx::ArraySlice<TChar> Characters) const
{
    auto result = State;
    for (auto& item : Characters)
        result = this->PerformTransition(result, item);
    return result;
}

/// \brief Applies the automaton's transition function to the given
/// state and symbol.
template<typename TState, typename TChar>
TState DFAutomaton<TState, TChar>::PerformTransition(TState State, TChar Character) const
{
    auto transFun = this->getTransitionFunction();
    return transFun.Apply(std::pair<TState, TChar>(State, Character));
}

/// \brief Gets the set of all reachable states in this automaton.
template<typename TState, typename TChar>
LinearSet<TState> DFAutomaton<TState, TChar>::ReachableStates() const
{
    LinearSet<TState> todo(this->getStartState());
    LinearSet<TState> reachable;
    while (!todo.getIsEmpty())
    {
        auto q = todo.Pop();
        reachable.Add(q);
        auto alpha = this->GetAlphabet();
        for (auto& sym : alpha.getItems())
        {
            auto newState = this->PerformTransition(q, sym);
            if (!reachable.Contains(newState))
                todo.Add(newState);

        }
    }
    return reachable;
}


/// \brief Performs the table-filling algorithm on this automaton,
/// and returns the resulting partition of equivalent states.
template<typename TState, typename TChar>
std::unordered_map<TState, LinearSet<TState>> DFAutomaton<TState, TChar>::TFAPartition() const
{
    auto states = this->GetStates();
    auto symbols = this->GetAlphabet();
    auto accepting = this->getAcceptingStates();
    LinearSet<std::pair<TState, TState>> statePairs;
    for (auto& a : states.getItems())
        for (auto& b : states.getItems())
            statePairs.Add(std::pair<TState, TState>(a, b));
    LinearSet<std::pair<TState, TState>> distinguishablePairs;
    for (auto& a0 : states.getItems())
        if (!accepting.Contains(a0))
            for (auto& b0 : accepting.getItems())
            {
                distinguishablePairs.Add(std::pair<TState, TState>(a0, b0));
                distinguishablePairs.Add(std::pair<TState, TState>(b0, a0));
            }

    auto unexaminedPairs = distinguishablePairs;
    while (!unexaminedPairs.getIsEmpty())
    {
        auto ab = unexaminedPairs.Pop();
        for (auto& sym : symbols.getItems())
            for (auto& pq : statePairs.getItems())
            {
                if (distinguishablePairs.Contains(pq))
                    continue;

                if (this->PerformTransition(pq.first, sym) == ab.first && 
                this->PerformTransition(pq.second, sym) == ab.second)
                {
                    distinguishablePairs.Add(pq);
                    unexaminedPairs.Add(pq);
                }
            }
    }
    std::unordered_map<TState, LinearSet<TState>> partition;
    for (auto& q : states.getItems())
        partition[q] = LinearSet<TState>(q);
    for (auto& ab0 : statePairs.getItems())
        if (!distinguishablePairs.Contains(ab0))
        {
            auto firstPart = partition[ab0.first];
            auto uni = firstPart.Union(partition[ab0.second]);
            partition[ab0.first] = uni;
            partition[ab0.second] = uni;
        }
    return partition;
}

/// \brief Gets this automaton's set of accepting states.
template<typename TState, typename TChar>
LinearSet<TState> DFAutomaton<TState, TChar>::getAcceptingStates() const
{
    return this->AcceptingStates_value;
}

/// \brief Sets this automaton's set of accepting states.
template<typename TState, typename TChar>
void DFAutomaton<TState, TChar>::setAcceptingStates(LinearSet<TState> value)
{
    this->AcceptingStates_value = value;
}

/// \brief Gets this automaton's start state.
template<typename TState, typename TChar>
TState DFAutomaton<TState, TChar>::getStartState() const
{
    return this->StartState_value;
}

/// \brief Sets this automaton's start state.
template<typename TState, typename TChar>
void DFAutomaton<TState, TChar>::setStartState(TState value)
{
    this->StartState_value = value;
}

/// \brief Gets this automaton's transition function.
template<typename TState, typename TChar>
TransitionTable<std::pair<TState, TChar>, TState> DFAutomaton<TState, TChar>::getTransitionFunction() const
{
    return this->TransitionFunction_value;
}

/// \brief Sets this automaton's transition function.
template<typename TState, typename TChar>
void DFAutomaton<TState, TChar>::setTransitionFunction(TransitionTable<std::pair<TState, TChar>, TState> value)
{
    this->TransitionFunction_value = value;
}