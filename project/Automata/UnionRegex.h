#pragma once
#include <memory>
#include <string>
#include "ENFAutomaton.h"
#include "IRegex.h"
#include "RegexState.h"

namespace Automata
{
    /// \brief Defines a regex type that is the union of two other regex.
    struct UnionRegex : public virtual IRegex
    {
        UnionRegex(std::shared_ptr<IRegex> Left, std::shared_ptr<IRegex> Right);

        /// \brief Creates an epsilon-nfa for this union regex.
        /// The construction is as follows:   * Automata are
        /// constructed for the left and right operands of this union
        /// operation.   * These automata are "merged":     * A new
        /// start state is created, with epsilon-transitions going out
        /// to the left and right automata.
        /// * The accepting states of the resulting automaton is
        /// union of the       accepting states of the left and right
        /// operands' automata.
        ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const override;

        /// \brief Gets this regex's string representation.
        std::string ToString() const override;

        std::shared_ptr<IRegex> Left;
        std::shared_ptr<IRegex> Right;
    };
}