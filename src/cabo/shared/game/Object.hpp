#pragma once

#include "core/object/Object.hpp"
#include "shared/board/Object.hpp"

namespace cn::shared::game
{

class Object : public core::object::Object
{
public:
    Object() = default;

    virtual board::Object& getBoardObject() const = 0;
};

} // namespace cn::shared::game
