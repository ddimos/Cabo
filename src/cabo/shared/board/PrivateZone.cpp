#include "shared/board/PrivateZone.hpp"

namespace cn::shared::board
{

PrivateZone::PrivateZone(ObjectId _id, PlayerId _ownerId)
    : Object(_id), m_ownerId(_ownerId)
{
}

} // namespace cn::shared::board
