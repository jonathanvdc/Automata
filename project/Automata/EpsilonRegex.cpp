#include <string>
#include <memory>
#include <unordered_map>
#include <utility>
#include "ENFAutomaton.h"
#include "RegexState.h"
#include "LinearSet.h"
#include "Optional.h"
#include "TransitionTable.h"
#include "IRegex.h"
#include "EpsilonRegex.h"
#include "HashExtensions.h"

using namespace Automata;

std::string EpsilonRegex::ToString() const
{
    return "\\e";
}

ENFAutomaton<std::shared_ptr<RegexState>, std::string> EpsilonRegex::ToENFAutomaton() const
{
    auto startState = std::make_shared<RegexState>();
    LinearSet<std::shared_ptr<RegexState>> acceptingStates;
    acceptingStates.Add(std::make_shared<RegexState>());
    std::unordered_map<std::pair<std::shared_ptr<RegexState>, Optional<std::string>>, LinearSet<std::shared_ptr<RegexState>>> transitions;
    std::pair<std::shared_ptr<RegexState>, Optional<std::string>> label(startState, Optional<std::string>());
    transitions[label] = acceptingStates;
    TransitionTable<std::pair<std::shared_ptr<RegexState>, Optional<std::string>>, LinearSet<std::shared_ptr<RegexState>>> transTable(transitions);
    return ENFAutomaton<std::shared_ptr<RegexState>, std::string>(startState, acceptingStates, transTable);
}

EpsilonRegex::EpsilonRegex()
{ }