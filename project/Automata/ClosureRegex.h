#pragma once
#include <memory>
#include <string>
#include "ENFAutomaton.h"
#include "IRegex.h"
#include "RegexState.h"

namespace Automata
{
    /// \brief Defines a regex type that represents the closure of a
    /// given regex, which matches the language of said regex zero
    /// or more times.
    struct ClosureRegex : public virtual IRegex
    {
        /// \brief Creates a new closure regex from the given regex.
        ClosureRegex(std::shared_ptr<IRegex> Regex);

        /// \brief Creates an epsilon-nfa for this closure regex.
        /// The construction is as follows:   * An automaton is
        /// constructed for the closure regex' inner regex.
        /// * A new start state and a single accepting state are
        /// created.   * An automaton is created that consists of:
        /// * The inner regex' automaton     * The new start and
        /// accepting states     * epsilon transitions that connect the
        /// inner automaton's start and       end states with the new
        /// automaton's start and end states.
        /// Also, a "loop" is created to facilitate the closure.
        ENFAutomaton<std::shared_ptr<RegexState>, std::string> ToENFAutomaton() const override;

        /// \brief Gets this regex's string representation.
        std::string ToString() const override;

        /// \brief The closure regex' inner regex.
        std::shared_ptr<IRegex> Regex;
    };
}