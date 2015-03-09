#include "Pair.h"

using namespace Automata;

/// \brief Creates a new immutable pair.
template<typename TFirst, typename TSecond>
Pair<TFirst, TSecond>::Pair(TFirst First, TSecond Second)
{
    this->setFirst(First);
    this->setSecond(Second);
}

template<typename TFirst, typename TSecond>
bool Pair<TFirst, TSecond>::operator==(Pair<TFirst, TSecond> Other) const
{
    return this->getFirst() == Other.getFirst() && this->getSecond() == Other.getSecond();
}

template<typename TFirst, typename TSecond>
bool Pair<TFirst, TSecond>::operator!=(Pair<TFirst, TSecond> Other) const
{
    return this->getFirst() != Other.getFirst() || this->getSecond() != Other.getSecond();
}

/// \brief Sets the pair's first element.
template<typename TFirst, typename TSecond>
void Pair<TFirst, TSecond>::setFirst(TFirst value)
{
    this->First_value = value;
}
/// \brief Gets the pair's first element.
template<typename TFirst, typename TSecond>
TFirst Pair<TFirst, TSecond>::getFirst() const
{
    return this->First_value;
}

/// \brief Sets the pair's second element.
template<typename TFirst, typename TSecond>
void Pair<TFirst, TSecond>::setSecond(TSecond value)
{
    this->Second_value = value;
}
/// \brief Gets the pair's second element.
template<typename TFirst, typename TSecond>
TSecond Pair<TFirst, TSecond>::getSecond() const
{
    return this->Second_value;
}