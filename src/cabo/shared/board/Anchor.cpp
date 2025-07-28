#include "shared/board/Anchor.hpp"

namespace cn::shared::board
{

Anchor::Anchor(ObjectId _id, int _radius)
    : Object(_id)
    , m_radius(_radius)
{
}

} // namespace cn::shared::board
