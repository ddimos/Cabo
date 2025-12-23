#pragma once

#include "shared/game/object/Object.hpp"

namespace cn::shared::game::component
{

class Component
{
public:
    Component(object::Object& _parent)
        : m_parentRef(_parent)
    {}

    object::Object& getParent() const { return m_parentRef; }

private:
    object::Object& m_parentRef;
};

} // namespace cn::shared::game::component
