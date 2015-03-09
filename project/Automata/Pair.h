#pragma once

namespace Automata
{
    /// \brief Defines an immutable pair.
    template<typename TFirst, typename TSecond>
    class Pair
    {
    public:
        /// \brief Creates a new immutable pair.
        Pair(TFirst First, TSecond Second);

        bool operator==(Pair<TFirst, TSecond> Other) const;

        bool operator!=(Pair<TFirst, TSecond> Other) const;

        /// \brief Sets the pair's first element.
        void setFirst(TFirst value);
        /// \brief Gets the pair's first element.
        TFirst getFirst() const;

        /// \brief Sets the pair's second element.
        void setSecond(TSecond value);
        /// \brief Gets the pair's second element.
        TSecond getSecond() const;
    private:
        TFirst First_value;
        TSecond Second_value;
    };
}

#include "Pair.hxx"