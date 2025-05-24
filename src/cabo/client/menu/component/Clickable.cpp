#include "client/menu/component/Clickable.hpp"

#include "core/Assert.hpp"

namespace cn::client::menu::component
{

void Clickable::setCallback(Callback _callback)
{
    m_onClickCallback = _callback;
}

void Clickable::setButton(sf::Mouse::Button _button)
{
    m_button = _button;
}

void Clickable::onMouseReleased(sf::Event::MouseButtonEvent _event)
{
    CN_ASSERT(m_button != sf::Mouse::Button::ButtonCount);
    if (_event.button != m_button)
        return;

    if (!contains(sf::Vector2f(_event.x, _event.y)))
    {
        m_isActive = false;
        return;
    }
    
    m_isActive = true;
    m_onClickCallback();
}

} // namespace cn::client::menu::component
