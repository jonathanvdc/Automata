#pragma once
#include <memory>
#include <string>
#include "ENFAutomaton.h"
#include "IRegex.h"
#include "RegexState.h"

namespace Automata
{
    /// \brief Defines a literal regex, which is a regex that matches a
    /// single symbol.
    struct LiteralRegex : public virtual IRegex
    {
        /// \brief Creates a literal regex for the given symbol.
        LiteralRegex(std::string Literal);

        /// \brief Constructs an epsilon-nfa for this regex.
        /// The construction is as follows:    start literal end.
        /// where 'start' is the initial state, 'end' is an accepting
        /// state, and 'literal' is this regex' literal.
        ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const override;

        /// \brief Gets this regex's string representation.
        std::string ToString() const override;

        /// \brief Gets the literal regex's literal or symbol.
        std::string Literal;
    };
}