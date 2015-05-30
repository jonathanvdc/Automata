#include "ConcatRegex.h"

#include <memory>
#include <string>
#include <utility>
#include "ENFAutomaton.h"
#include "IRegex.h"
#include "IsInstance.h"
#include "LinearSet.h"
#include "Optional.h"
#include "RegexState.h"
#include "TransitionTable.h"
#include "UnionRegex.h"

using namespace Automata;

/// \brief Creates a new concat regex for the given operands.
ConcatRegex::ConcatRegex(std::shared_ptr<IRegex> Left, std::shared_ptr<IRegex> Right)
    : Left(Left), Right(Right)
{ }

/// \brief Creates an epsilon-nfa for this concatenation regex.
/// The construction is as follows:   * Automata are
/// constructed for the left and right operands of this concat
/// operation.   * These automata are "merged":     * The start
/// state of the resulting automaton is the start state
/// of the left operand's automaton.
/// * The accepting states of the resulting automaton is
/// the set of accepting       states of the right operand's
/// automaton.   * For each accepting state in the left
/// automaton, an epsilon-transition     is added to the
/// resulting automaton that starts at said accepting     state
/// and points to the right automaton's intial state.
ENFAutomaton<std::shared_ptr<RegexState>, std::string> ConcatRegex::ToENFAutomaton() const
{
    TransitionTable<std::pair<std::shared_ptr<RegexState>, Optional<std::string>>, LinearSet<std::shared_ptr<RegexState>>> transTable;
    auto leftAutomaton = this->Left->ToENFAutomaton();
    auto rightAutomaton = this->Right->ToENFAutomaton();
    auto startState = std::make_shared<RegexState>();
    transTable.Add(leftAutomaton.getTransitionFunction());
    transTable.Add(rightAutomaton.getTransitionFunction());
    std::pair<std::shared_ptr<RegexState>, Optional<std::string>> label(startState, 
                                                                        Optional<std::string>());
    LinearSet<std::shared_ptr<RegexState>> leftStartStates;
    leftStartStates.Add(leftAutomaton.getStartState());
    transTable.Add(label, leftStartStates);
    auto leftEndStates = leftAutomaton.getAcceptingStates();
    LinearSet<std::shared_ptr<RegexState>> rightStartStates;
    rightStartStates.Add(rightAutomaton.getStartState());
    for (auto& item : leftEndStates.getItems())
    {
        std::pair<std::shared_ptr<RegexState>, Optional<std::string>> pipeTransition(item, 
                                                                                     Optional<std::string>());
        transTable.Add(pipeTransition, rightStartStates);
    }
    auto acceptingStates = rightAutomaton.getAcceptingStates();
    return ENFAutomaton<std::shared_ptr<RegexState>, std::string>(startState, 
                                                                  acceptingStates, 
                                                                  transTable);
}

/// \brief Gets this regex's string representation.
std::string ConcatRegex::ToString() const
{
    std::string result;
    if (stdx::isinstance<UnionRegex>(this->Left))
        result = "(" + this->Left->ToString() + ")";
    else
        result = this->Left->ToString();

    if (stdx::isinstance<UnionRegex>(this->Right))
        result += "(" + this->Right->ToString() + ")";
    else
        result += this->Right->ToString();

    return result;
}