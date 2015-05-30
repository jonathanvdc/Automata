#pragma once
#include <memory>
#include <string>
#include "ENFAutomaton.h"
#include "IRegex.h"
#include "RegexState.h"

namespace Automata
{
    /// \brief A regular expression that represents the concatenation of
    /// two other regular expressions.
    struct ConcatRegex : public virtual IRegex
    {
        /// \brief Creates a new concat regex for the given operands.
        ConcatRegex(std::shared_ptr<IRegex> Left, std::shared_ptr<IRegex> Right);

        /// \brief Creates an epsilon-nfa for this concatenation regex.
        /// The construction is as follows:   * Automata are
        /// constructed for the left and right operands of this concat
        /// operation.   * These automata are "merged":     * The start
        /// state of the resulting automaton is the start state
        /// of the left operand's automaton.
        /// * The accepting states of the resulting automaton is
        /// the set of accepting       states of the right operand's
        /// automaton.   * For each accepting state in the left
        /// automaton, an epsilon-transition     is added to the
        /// resulting automaton that starts at said accepting     state
        /// and points to the right automaton's intial state.
        ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const override;

        /// \brief Gets this regex's string representation.
        std::string ToString() const override;

        /// \brief The concat regex's left operand.
        std::shared_ptr<IRegex> Left;
        /// \brief The concat regex's right operand.
        std::shared_ptr<IRegex> Right;
    };
}