#include "client/menu/component/Hoverable.hpp"

namespace cn::client::menu::component
{

void Hoverable::setCallback(Callback _callback)
{
    m_onHoverCallback = _callback;
}

void Hoverable::onMouseMoved(sf::Event::MouseMoveEvent _event)
{
    if (contains(sf::Vector2f(_event.x, _event.y)))
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

} // namespace cn::client::menu::component
