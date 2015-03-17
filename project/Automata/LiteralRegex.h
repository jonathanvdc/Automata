#pragma once
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

namespace Automata
{
    struct LiteralRegex : public IRegex
    {
        LiteralRegex(std::string Literal);

        std::string ToString() const override;

        ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const override;

        std::string Literal;
    };
}