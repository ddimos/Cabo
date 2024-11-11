#include "menu/component/Hoverable.hpp"

namespace cn::menu::component
{

void Hoverable::setCallback(Callback _callback)
{
    m_onHoverCallback = _callback;
}

void Hoverable::setBounds(sf::FloatRect _rect)
{
    m_bounds = _rect;
}

void Hoverable::onMouseMoved(sf::Event::MouseMoveEvent _event)
{
    const sf::Vector2f mousePosition(_event.x, _event.y);
    if (m_bounds.contains(mousePosition))
    {
        if (!m_isHovered)
        {
            m_isHovered = true;
            m_onHoverCallback(true);
        }
    }
    else
    {
        if (m_isHovered)
        {
            m_isHovered = false;
            m_onHoverCallback(false);
        }
    }
}

} // namespace cn::menu::component
