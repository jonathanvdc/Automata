#include <unordered_map>
#include "IFunction.h"
#include "UnorderedMapFunction.h"

using namespace Automata;

/// \brief Applies the function to the given value.
template<typename TSource, typename TTarget>
TTarget UnorderedMapFunction<TSource, TTarget>::Apply(TSource Value) const
{
    return this->getMap()[Value];
}

template<typename TSource, typename TTarget>
UnorderedMapFunction<TSource, TTarget>::UnorderedMapFunction(std::unordered_map<TSource, TTarget> Map)
{
    this->setMap(Map);
}

template<typename TSource, typename TTarget>
std::unordered_map<TSource, TTarget> UnorderedMapFunction<TSource, TTarget>::getMap() const
{
    return this->Map_value;
}
template<typename TSource, typename TTarget>
void UnorderedMapFunction<TSource, TTarget>::setMap(std::unordered_map<TSource, TTarget> value)
{
    this->Map_value = value;
}