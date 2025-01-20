#pragma once

#include "core/object/Object.hpp"
#include "client/menu/component/Transformable.hpp"

namespace cn::client::menu
{

class Item : public core::object::Object
{
protected:
    component::Transformable m_transformable;
};

} // namespace cn::client::menu
