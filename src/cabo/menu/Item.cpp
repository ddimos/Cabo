#include "menu/Item.hpp"

namespace cn::menu
{

Item::~Item() = default;

void Item::handleEvent(const sf::Event& _event)
{
    if (!m_isActivated)
        return;
    onHandleEvent(_event);
}

void Item::update(sf::Time _dt)
{
    if (!m_isActivated)
        return;
    onUpdate(_dt);
}

void Item::draw(sf::RenderWindow& _window)
{
    if (!m_isActivated)
        return;
    onDraw(_window);
}

void Item::activate()
{
    m_isActivated = true;
    onActivate();
}

void Item::deactivate()
{
    m_isActivated = false;
    onDeactivate();
}

} // namespace cn::menu
