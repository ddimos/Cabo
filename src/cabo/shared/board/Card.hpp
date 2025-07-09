#pragma once

#include "shared/board/Object.hpp"
#include "core/Identifier.hpp"

namespace cn::shared::board
{

class Card : public Object
{
public:
    using Value = core::Identifier<Object, uint8_t>;

    Card(ObjectId _id);

    Value getValue() const { return m_value; }
    void setValue(Value _value) { m_value = _value; }

private:
    Value m_value{};
};

} // namespace cn::shared::board
