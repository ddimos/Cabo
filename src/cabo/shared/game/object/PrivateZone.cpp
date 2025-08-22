#include "shared/game/object/PrivateZone.hpp"

namespace cn::shared::game::object
{

PrivateZone::PrivateZone(Id _id, PlayerId _ownerId)
    : Object(_id), m_ownerId(_ownerId)
{
}

} // namespace cn::shared::game::object
