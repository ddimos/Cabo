#include "shared/game/object/Object.hpp"
#include "core/math/Math.hpp"

namespace cn::shared::game::object
{

Object::Object(Id _id)
    : m_id(_id)
{
}

bool Object::contains(sf::Vector2f _pos) const
{
    const sf::Vector2f positionInLocalCoord = core::math::rotateVectorInverse((_pos - m_bounds.getPosition()), m_rotation);
    sf::FloatRect localBounds(-m_bounds.getSize() / 2.f, m_bounds.getSize());
    return localBounds.contains(positionInLocalCoord);
}

} // namespace cn::shared::game::object
