#pragma once
#include <memory>
#include <string>
#include <utility>
#include "IRegex.h"
#include "ENFAutomaton.h"
#include "RegexState.h"
#include "TransitionTable.h"
#include "Optional.h"
#include "LinearSet.h"

namespace Automata
{
    struct ClosureRegex : public IRegex
    {
        ClosureRegex(std::shared_ptr<IRegex> Regex);

        std::string ToString() const override;

        ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const override;

        std::shared_ptr<IRegex> Regex;
    };
}