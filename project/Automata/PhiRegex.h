#pragma once
#include <string>
#include <memory>
#include <utility>
#include "ENFAutomaton.h"
#include "RegexState.h"
#include "LinearSet.h"
#include "TransitionTable.h"
#include "Optional.h"
#include "IRegex.h"

namespace Automata
{
    struct PhiRegex : public IRegex
    {
        PhiRegex();

        std::string ToString() const override;

        ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const override;
    };
}