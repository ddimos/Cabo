#include "shared/game/object/Anchor.hpp"

namespace cn::shared::game::object
{

Anchor::Anchor(Id _id, int _radius)
    : Object(_id)
    , m_radius(_radius)
{
}

} // namespace cn::shared::game::object
