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
    struct UnionRegex : public IRegex
    {
        UnionRegex(std::shared_ptr<IRegex> Left, std::shared_ptr<IRegex> Right);

        std::string ToString() const override;

        ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const override;

        std::shared_ptr<IRegex> Left;
        std::shared_ptr<IRegex> Right;
    };
}