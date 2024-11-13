#pragma once

#include "core/object/Object.hpp"
#include "menu/component/Transformable.hpp"

namespace cn::menu
{

class Item : public core::object::Object
{
protected:
    component::Transformable m_transformable;
};

} // namespace cn::menu
