#include "Optional.h"

#include <functional>

template<typename T>
Optional<T>::Optional()
    : HasValue(false)
{ }

template<typename T>
Optional<T>::Optional(T Value)
    : Value(Value), HasValue(true)
{ }

template<typename T>
int Optional<T>::GetHashCode() const
{
    if (!this->HasValue)
        return 0;
    else
        return (int)std::hash<T>()(this->Value);

}

template<typename T>
bool Optional<T>::operator==(Optional<T> Other) const
{
    if (this->HasValue)
        return Other.HasValue && this->Value == Other.Value;
    else
        return !Other.HasValue;

}

template<typename T>
bool Optional<T>::operator==(T Other) const
{
    return this->HasValue && this->Value == Other;
}

template<typename T>
bool Optional<T>::operator!=(Optional<T> Other) const
{
    if (!this->HasValue)
        return Other.HasValue;
    else
        return !Other.HasValue || this->Value != Other.Value;

}

template<typename T>
bool Optional<T>::operator!=(T Other) const
{
    return !this->HasValue || this->Value == Other;
}