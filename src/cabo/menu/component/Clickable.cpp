#include "menu/component/Clickable.hpp"

#include "core/Assert.hpp"

namespace cn::menu::component
{

void Clickable::setCallback(Callback _callback)
{
    m_onClickCallback = _callback;
}

void Clickable::setButton(sf::Mouse::Button _button)
{
    m_button = _button;
}

void Clickable::setBounds(sf::FloatRect _rect)
{
    m_bounds = _rect;
}

void Clickable::onMouseReleased(sf::Event::MouseButtonEvent _event)
{
    const sf::Vector2f mousePosition(_event.x, _event.y);
    if (!m_bounds.contains(mousePosition))
        return;
    
    CN_ASSERT(m_button != sf::Mouse::Button::ButtonCount);

    if (_event.button != m_button)
        return;
    
    m_onClickCallback(true);
}

} // namespace cn::menu::component
