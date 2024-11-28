#include "core/object/Object.hpp"
#include "core/Assert.hpp"

namespace cn::core::object
{

Object::~Object() = default;

void Object::setActivationOption(ActivationOption _activationOption)
{
    m_activationOption = _activationOption;
}

bool Object::isAutoActivated() const
{
    return m_activationOption == ActivationOption::Auto;
}

void Object::activate()
{
    CN_ASSERT(!m_isActivated);
    if (m_isActivated)
        return;
    m_isActivated = true;
    onActivate();
}

void Object::deactivate()
{
    // Don't need to assert if not activated
    if (!m_isActivated)
        return;
    m_isActivated = false;
    onDeactivate();
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

void Object::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    onRegisterEvents(_dispatcher, _isBeingRegistered);
}

} // namespace cn::core::object
