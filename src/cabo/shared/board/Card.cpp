#include "shared/board/Card.hpp"

namespace cn::shared::board
{

Card::Card(ObjectId _id)
    : Object(_id)
    , m_flippableComponent(*this)
    , m_grabbableComponent(*this)
{
}

} // namespace cn::shared::board
