#pragma once
#include <memory>
#include <string>
#include "ENFAutomaton.h"
#include "RegexState.h"

namespace Automata
{
    /// \brief Defines a generic regular expression.
    struct IRegex
    {
        /// \brief Constructs an epsilon-nfa for this regex.
        virtual ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const = 0;

        /// \brief Gets this regex's string representation.
        virtual std::string ToString() const = 0;
    };
}