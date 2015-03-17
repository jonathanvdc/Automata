#pragma once
#include <memory>
#include <string>
#include <utility>
#include "IRegex.h"
#include "UnionRegex.h"
#include "IsInstance.h"
#include "ENFAutomaton.h"
#include "RegexState.h"
#include "TransitionTable.h"
#include "Optional.h"
#include "LinearSet.h"

namespace Automata
{
    struct ConcatRegex : public IRegex
    {
        ConcatRegex(std::shared_ptr<IRegex> Left, std::shared_ptr<IRegex> Right);

        std::string ToString() const override;

        ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const override;

        std::shared_ptr<IRegex> Left;
        std::shared_ptr<IRegex> Right;
    };
}