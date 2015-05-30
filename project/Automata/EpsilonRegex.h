#pragma once
#include <memory>
#include <string>
#include "ENFAutomaton.h"
#include "IRegex.h"
#include "RegexState.h"

namespace Automata
{
    /// \brief Defines an epsilon regex, which matches the empty string.
    struct EpsilonRegex : public virtual IRegex
    {
        EpsilonRegex();

        /// \brief Constructs an epsilon-nfa for this regex.
        /// The construction is as follows:    start -> end.
        /// where 'start' is the initial state, 'end' is an accepting
        /// state, and '->' is an epsilon-transition.
        ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const override;

        /// \brief Gets this regex's string representation.
        std::string ToString() const override;
    };
}