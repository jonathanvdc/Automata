#pragma once
#include <vector>
#include <initializer_list>
#include "ArraySlice.h"

template<typename T>
class LinearSet
{
public:
    LinearSet();
    LinearSet(std::vector<T> Values);
    LinearSet(stdx::ArraySlice<T> Values);
    LinearSet(std::initializer_list<T> Values);

    bool Contains(T Value) const;

    bool ContainsAll(LinearSet<T> Values) const;

    void Add(T Value);

    void AddAll(LinearSet<T> Other);

    void RemoveLast();

    LinearSet<T> Union(LinearSet<T> Other) const;

    LinearSet<T> Intersect(LinearSet<T> Other) const;

    LinearSet<T> Without(T Value) const;

    bool operator==(LinearSet<T> Other) const;

    bool operator!=(LinearSet<T> Other) const;

    std::vector<T> getItems() const;

    int getCount() const;

    bool getIsEmpty() const;

    T getLast() const;
private:
    std::vector<T> vals;
};

#include "LinearSet.hxx"