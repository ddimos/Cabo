#pragma once

#include "shared/game/object/Object.hpp"
#include "shared/player/Types.hpp"

namespace cn::shared::game::object
{

class PrivateZone : public Object
{
public:
    PrivateZone(Id _id, PlayerId _ownerId);

    PlayerId getOwnerId() const { return m_ownerId; }

private:
    PlayerId m_ownerId{};
};

} // namespace cn::shared::game::object
