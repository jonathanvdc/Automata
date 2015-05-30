#include "PhiRegex.h"

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

PhiRegex::PhiRegex()
{ }

/// \brief Constructs an epsilon-nfa for this regex.
/// The construction is as follows:    start.
/// where 'start' is the initial state.
/// There are no transitions, and no accepting states.
ENFAutomaton<std::shared_ptr<RegexState>, std::string> PhiRegex::ToENFAutomaton() const
{
    auto startState = std::make_shared<RegexState>();
    LinearSet<std::shared_ptr<RegexState>> acceptingStates;
    TransitionTable<std::pair<std::shared_ptr<RegexState>, Optional<std::string>>, LinearSet<std::shared_ptr<RegexState>>> transTable;
    return ENFAutomaton<std::shared_ptr<RegexState>, std::string>(startState, 
                                                                  acceptingStates, 
                                                                  transTable);
}

/// \brief Gets this regex's string representation.
std::string PhiRegex::ToString() const
{
    return "\\p";
}