#pragma once
#include <unordered_map>
#include <utility>
#include "IFunction.h"

namespace Automata
{
    /// \brief Defines a transition table that uses an unordered_map under the hood.
    template<typename TSource, typename TTarget>
    class TransitionTable : public IFunction<TSource, TTarget>
    {
    public:
        TransitionTable(std::unordered_map<TSource, TTarget> table);
        TransitionTable();

        /// \brief Applies the function to the given value.
        TTarget Apply(TSource Value) const override;

        void Add(TSource Source, TTarget Target);

        void Add(std::unordered_map<TSource, TTarget> Transitions);

        void Add(TransitionTable<TSource, TTarget> Transitions);

        std::unordered_map<TSource, TTarget> getMap() const;
    private:
        std::unordered_map<TSource, TTarget> table;
    };
}

#include "TransitionTable.hxx"