#pragma once

#include "shared/board/Object.hpp"
#include "shared/player/Types.hpp"

namespace cn::shared::board
{

class PrivateZone : public Object
{
public:
    PrivateZone(ObjectId _id, PlayerId _ownerId);

    PlayerId getOwnerId() const { return m_ownerId; }

private:
    PlayerId m_ownerId{};
};

} // namespace cn::shared::board
