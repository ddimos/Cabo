#include "menu/component/MouseInteractable.hpp"

#include "core/math/Math.hpp"

namespace cn::menu::component
{

void MouseInteractable::setBounds(sf::FloatRect _rect)
{
    m_bounds = _rect;
}

void MouseInteractable::setRotation(float _rotation)
{
    m_rotation = _rotation;
}

bool MouseInteractable::contains(sf::Vector2f _mousePos) const
{
    const sf::Vector2f mousePositionInLocalCoord = core::math::rotateVectorInverse((_mousePos - m_bounds.getPosition()), m_rotation);
    sf::FloatRect localBounds(sf::Vector2f(0.f, 0.f), m_bounds.getSize());
    return localBounds.contains(mousePositionInLocalCoord);
}

} // namespace cn::menu::component
