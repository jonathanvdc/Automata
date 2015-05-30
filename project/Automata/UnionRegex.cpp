#include "UnionRegex.h"

#include <memory>
#include <string>
#include <utility>
#include "ENFAutomaton.h"
#include "IRegex.h"
#include "LinearSet.h"
#include "Optional.h"
#include "RegexState.h"
#include "TransitionTable.h"

using namespace Automata;

UnionRegex::UnionRegex(std::shared_ptr<IRegex> Left, std::shared_ptr<IRegex> Right)
    : Left(Left), Right(Right)
{ }

/// \brief Creates an epsilon-nfa for this union regex.
/// The construction is as follows:   
/// * Automata are constructed for the left and right operands of this union
///   operation.   
/// * These automata are "merged":     
///   * A new start state is created, with epsilon-transitions going out
///     to the left and right automata.
/// * The accepting states of the resulting automaton is
///   union of the accepting states of the left and right
///   operands' automata.
ENFAutomaton<std::shared_ptr<RegexState>, std::string> UnionRegex::ToENFAutomaton() const
{
    auto startState = std::make_shared<RegexState>();
    TransitionTable<std::pair<std::shared_ptr<RegexState>, Optional<std::string>>, LinearSet<std::shared_ptr<RegexState>>> transTable;
    auto leftAutomaton = this->Left->ToENFAutomaton();
    auto rightAutomaton = this->Right->ToENFAutomaton();
    transTable.Add(leftAutomaton.getTransitionFunction());
    transTable.Add(rightAutomaton.getTransitionFunction());
    std::pair<std::shared_ptr<RegexState>, Optional<std::string>> label(startState, 
                                                                        Optional<std::string>());
    LinearSet<std::shared_ptr<RegexState>> redirectedStates;
    redirectedStates.Add(leftAutomaton.getStartState());
    redirectedStates.Add(rightAutomaton.getStartState());
    transTable.Add(label, redirectedStates);
    auto acceptingStates = leftAutomaton.getAcceptingStates();
    acceptingStates.AddAll(rightAutomaton.getAcceptingStates());
    return ENFAutomaton<std::shared_ptr<RegexState>, std::string>(startState, 
                                                                  acceptingStates, 
                                                                  transTable);
}

/// \brief Gets this regex's string representation.
std::string UnionRegex::ToString() const
{
    return this->Left->ToString() + "+" + this->Right->ToString();
}