#pragma once

#include "shared/board/Object.hpp"

namespace cn::shared::board::component
{

class Component
{
public:
    Component(Object& _parent)
        : m_parentRef(_parent)
    {}

    Object& getParent() const { return m_parentRef; }

private:
    Object& m_parentRef;
};

} // namespace cn::shared::board::component
