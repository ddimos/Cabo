#include "core/object/Object.hpp"

namespace cn::core::object
{

Object::~Object() = default;

void Object::handleEvent(const sf::Event& _event)
{
    if (!m_isActivated)
        return;
    onHandleEvent(_event);
}

void Object::update(sf::Time _dt)
{
    if (!m_isActivated)
        return;
    onUpdate(_dt);
}

void Object::draw(sf::RenderWindow& _window)
{
    if (!m_isActivated)
        return;
    onDraw(_window);
}

void Object::activate()
{
    m_isActivated = true;
    onActivate();
}

void Object::deactivate()
{
    m_isActivated = false;
    onDeactivate();
}

} // namespace cn::core::object
