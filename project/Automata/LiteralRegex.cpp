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
#include "LiteralRegex.h"
#include "HashExtensions.h"

using namespace Automata;

std::string LiteralRegex::ToString() const
{
    return this->Literal;
}

ENFAutomaton<std::shared_ptr<RegexState>, std::string> LiteralRegex::ToENFAutomaton() const
{
    auto startState = std::make_shared<RegexState>();
    LinearSet<std::shared_ptr<RegexState>> acceptingStates;
    acceptingStates.Add(std::make_shared<RegexState>());
    std::unordered_map<std::pair<std::shared_ptr<RegexState>, Optional<std::string>>, LinearSet<std::shared_ptr<RegexState>>> transitions;
    std::pair<std::shared_ptr<RegexState>, Optional<std::string>> label(startState, Optional<std::string>(this->Literal));
    transitions[label] = acceptingStates;
    TransitionTable<std::pair<std::shared_ptr<RegexState>, Optional<std::string>>, LinearSet<std::shared_ptr<RegexState>>> transTable(transitions);
    return ENFAutomaton<std::shared_ptr<RegexState>, std::string>(startState, acceptingStates, transTable);
}

LiteralRegex::LiteralRegex(std::string Value)
{
	this->Literal = Value;
}