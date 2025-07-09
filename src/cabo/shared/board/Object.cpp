#include "shared/board/Object.hpp"

#include "core/math/Math.hpp"

namespace cn::shared::board
{

Object::Object(ObjectId _id)
    : m_id(_id)
{
}



bool Object::contains(sf::Vector2f _pos) const
{
    const sf::Vector2f positionInLocalCoord = core::math::rotateVectorInverse((_pos - m_bounds.getPosition()), m_rotation);
    sf::FloatRect localBounds(-m_bounds.getSize() / 2.f, m_bounds.getSize());
    return localBounds.contains(positionInLocalCoord);
}

void Object::onGrabbed()
{
    m_isGrabbed = true;
}

void Object::onReleased()
{
    m_isGrabbed = false;
}

} // namespace cn::shared::board
