#pragma once

template<typename T>
struct Optional
{
    Optional(T Value);
    Optional();

    bool operator==(Optional<T> Other) const;

    bool operator==(T Other) const;

    bool operator!=(Optional<T> Other) const;

    bool operator!=(T Other) const;

    bool HasValue;
    T Value;
};

#include "Optional.hxx"