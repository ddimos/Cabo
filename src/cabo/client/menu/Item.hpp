#pragma once

#include "core/object/Object.hpp"
#include "client/menu/component/Transformable.hpp"

namespace cn::client::menu
{

class Item : public core::object::Object
{
public:
    Position getPosition() const;
    void setPosition(Position _position);
protected:
    virtual void onPositionUpdated() {}

    component::Transformable m_transformable;
};

} // namespace cn::client::menu
