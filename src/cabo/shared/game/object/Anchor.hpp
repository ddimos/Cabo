#pragma once
#include "shared/game/object/Object.hpp"

namespace cn::shared::game::object
{

class Anchor : public Object
{
public:
    Anchor(Id _id, int _radius);

    int getRadius() const { return m_radius; }

private:
    int m_radius = 0;
};

} // namespace cn::shared::game::object
