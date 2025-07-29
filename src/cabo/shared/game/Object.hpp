#pragma once

#include "core/object/Object.hpp"
#include "shared/board/Object.hpp"

namespace cn::shared::game
{

class Object : public core::object::Object
{
public:
    Object(board::Object& _boardObject)
        : m_boardObjectRef(_boardObject)
    {}

    board::Object& getBoardObject() const { return m_boardObjectRef; }

private:
    board::Object& m_boardObjectRef;
};

} // namespace cn::shared::game
