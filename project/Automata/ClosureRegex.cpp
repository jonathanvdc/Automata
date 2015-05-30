#include "ClosureRegex.h"

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

/// \brief Creates a new closure regex from the given regex.
ClosureRegex::ClosureRegex(std::shared_ptr<IRegex> Regex)
    : Regex(Regex)
{ }

/// \brief Creates an epsilon-nfa for this closure regex.
/// The construction is as follows:   * An automaton is
/// constructed for the closure regex' inner regex.
/// * A new start state and a single accepting state are
///   created.   
/// * An automaton is created that consists of:
///   * The inner regex' automaton     
///   * The new start and accepting states     
///   * epsilon transitions that connect the
///     inner automaton's start and end states with the new
///     automaton's start and end states.
/// Also, a "loop" is created to facilitate the closure.
ENFAutomaton<std::shared_ptr<RegexState>, std::string> ClosureRegex::ToENFAutomaton() const
{
    TransitionTable<std::pair<std::shared_ptr<RegexState>, Optional<std::string>>, LinearSet<std::shared_ptr<RegexState>>> transTable;
    auto innerAutomaton = this->Regex->ToENFAutomaton();
    transTable.Add(innerAutomaton.getTransitionFunction());
    auto startState = std::make_shared<RegexState>();
    auto endState = std::make_shared<RegexState>();
    std::pair<std::shared_ptr<RegexState>, Optional<std::string>> label(startState,
                                                                        Optional<std::string>());
    LinearSet<std::shared_ptr<RegexState>> starTrans;
    starTrans.Add(innerAutomaton.getStartState());
    starTrans.Add(endState);
    transTable.Add(label, starTrans);
    auto innerEndStates = innerAutomaton.getAcceptingStates();
    for (auto& item : innerEndStates.getItems())
    {
        std::pair<std::shared_ptr<RegexState>, Optional<std::string>> pipeLabel(item,
                                                                                Optional<std::string>());
        transTable.Add(pipeLabel, starTrans);
    }
    LinearSet<std::shared_ptr<RegexState>> acceptingStates;
    acceptingStates.Add(endState);
    return ENFAutomaton<std::shared_ptr<RegexState>, std::string>(startState,
                                                                  acceptingStates,
                                                                  transTable);
}

/// \brief Gets this regex's string representation.
std::string ClosureRegex::ToString() const
{
    return "(" + this->Regex->ToString() + ")*";
}
