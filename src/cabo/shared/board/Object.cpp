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

InteractableObject::InteractableObject(ObjectId _id)
    : Object(_id)
{
}

void InteractableObject::grab()
{
    m_isGrabbed = true;
}

void InteractableObject::release()
{
    m_isGrabbed = false;
}

void InteractableObject::turnUp()
{
    m_isFaceUp = true;
}

void InteractableObject::turnDown()
{
    m_isFaceUp = false;
}

} // namespace cn::shared::board
