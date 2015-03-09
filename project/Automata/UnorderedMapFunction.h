#pragma once
#include <unordered_map>
#include "IFunction.h"

namespace Automata
{
    /// \brief Defines a pure function that uses an unordered_map under the hood.
    template<typename TSource, typename TTarget>
    class UnorderedMapFunction : public IFunction<TSource, TTarget>
    {
    public:
        UnorderedMapFunction(std::unordered_map<TSource, TTarget> Map);

        /// \brief Applies the function to the given value.
        TTarget Apply(TSource Value) const override;

        std::unordered_map<TSource, TTarget> getMap() const;
        void setMap(std::unordered_map<TSource, TTarget> value);
    private:
        std::unordered_map<TSource, TTarget> Map_value;
    };
}

#include "UnorderedMapFunction.hxx"