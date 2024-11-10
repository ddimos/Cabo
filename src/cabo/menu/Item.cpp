#include "menu/Item.hpp"

namespace cn::menu
{

Position::Position() = default;

Position::Position(sf::Vector2f _position)
    : m_position{_position}
{
}

Position::Position(Special _specPositionX, Special _specPositionY)
    : m_specPositionX{_specPositionX}
    , m_specPositionY{_specPositionY}
{
}

Position::Position(Special _specPositionX, Special _specPositionY, sf::Vector2f _position)
    : m_position{_position}
    , m_specPositionX{_specPositionX}
    , m_specPositionY{_specPositionY}
{
}

// ---------------------------------------------------------

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
