#pragma once
#include <memory>
#include <string>
#include "ENFAutomaton.h"
#include "IRegex.h"
#include "RegexState.h"

namespace Automata
{
    /// \brief Defines a phi regex, which matches nothing.
    struct PhiRegex : public virtual IRegex
    {
        PhiRegex();

        /// \brief Constructs an epsilon-nfa for this regex.
        /// The construction is as follows:    start.
        /// where 'start' is the initial state.
        /// There are no transitions, and no accepting states.
        ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const override;

        /// \brief Gets this regex's string representation.
        std::string ToString() const override;
    };
}