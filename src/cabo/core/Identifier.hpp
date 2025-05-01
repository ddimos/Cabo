#pragma once

#include <limits>
#include <cstdint>
#include <type_traits>

namespace cn::core
{

template<class Tag, class T, T DefaultValue = std::numeric_limits<T>::max()>
class Identifier final
{
    static_assert(std::is_integral<T>::value, "Identifier holds only integral types");
public:
    static Identifier invalid() { return Identifier(); }

    using Type = T;

    Identifier()
     : m_id(DefaultValue)
    {}

    explicit Identifier(T _id)
     : m_id(_id)
    {}

    T value() const { return m_id; }
    bool isValid() const { return m_id != DefaultValue; }

    friend bool operator<(Identifier _lhs, Identifier _rhs) { return _lhs.m_id < _rhs.m_id; }
    friend bool operator==(Identifier _lhs, Identifier _rhs) { return _lhs.m_id == _rhs.m_id; }
    friend bool operator!=(Identifier _lhs, Identifier _rhs) { return _lhs.m_id != _rhs.m_id; }

private:
    T m_id;
};

} // namespace cn::core
