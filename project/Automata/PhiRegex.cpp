#include <string>
#include <memory>
#include <utility>
#include "ENFAutomaton.h"
#include "RegexState.h"
#include "LinearSet.h"
#include "TransitionTable.h"
#include "Optional.h"
#include "IRegex.h"
#include "PhiRegex.h"
#include "HashExtensions.h"

using namespace Automata;

std::string PhiRegex::ToString() const
{
    return "\\p";
}

ENFAutomaton<std::shared_ptr<RegexState>, std::string> PhiRegex::ToENFAutomaton() const
{
    auto startState = std::make_shared<RegexState>();
    LinearSet<std::shared_ptr<RegexState>> acceptingStates;
    TransitionTable<std::pair<std::shared_ptr<RegexState>, Optional<std::string>>, LinearSet<std::shared_ptr<RegexState>>> transTable;
    return ENFAutomaton<std::shared_ptr<RegexState>, std::string>(startState, acceptingStates, transTable);
}

PhiRegex::PhiRegex()
{ }