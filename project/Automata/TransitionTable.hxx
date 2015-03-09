#include <unordered_map>
#include "IFunction.h"
#include "TransitionTable.h"

using namespace Automata;

/// \brief Applies the function to the given value.
template<typename TSource, typename TTarget>
TTarget TransitionTable<TSource, TTarget>::Apply(TSource Value) const
{
    return this->getMap()[Value];
}

template<typename TSource, typename TTarget>
TransitionTable<TSource, TTarget>::TransitionTable(std::unordered_map<TSource, TTarget> Map)
{
    this->setMap(Map);
}

template<typename TSource, typename TTarget>
TransitionTable<TSource, TTarget>::TransitionTable()
{ }

template<typename TSource, typename TTarget>
std::unordered_map<TSource, TTarget> TransitionTable<TSource, TTarget>::getMap() const
{
    return this->Map_value;
}
template<typename TSource, typename TTarget>
void TransitionTable<TSource, TTarget>::setMap(std::unordered_map<TSource, TTarget> value)
{
    this->Map_value = value;
}