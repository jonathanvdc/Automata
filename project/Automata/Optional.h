#pragma once
#include <functional>

template<typename T>
struct Optional
{
    Optional();
    Optional(T Value);

    int GetHashCode() const;

    bool operator==(Optional<T> Other) const;

    bool operator==(T Other) const;

    bool operator!=(Optional<T> Other) const;

    bool operator!=(T Other) const;

    bool HasValue = false;
    T Value;
};
namespace std
{
    template<typename T>
    struct hash<Optional<T>>
    {
        std::size_t operator()(const Optional<T>& Arg) const
        {
            return Arg.GetHashCode();
        }
    };
}

#include "Optional.hxx"