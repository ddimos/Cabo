#include "shared/board/BoardObject.hpp"

#include "core/math/Math.hpp"

namespace cn::shared::board
{

BoardObject::BoardObject(BoardObjectId _id)
    : m_id(_id)
{
}



bool BoardObject::contains(sf::Vector2f _pos) const
{
    const sf::Vector2f positionInLocalCoord = core::math::rotateVectorInverse((_pos - m_bounds.getPosition()), m_rotation);
    sf::FloatRect localBounds(-m_bounds.getSize() / 2.f, m_bounds.getSize());
    return localBounds.contains(positionInLocalCoord);
}

void BoardObject::onGrabbed()
{
    m_isGrabbed = true;
}

void BoardObject::onReleased()
{
    m_isGrabbed = false;
}

} // namespace cn::shared::board
