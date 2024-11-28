#include "core/state/State.hpp"
#include "core/state/Manager.hpp"

namespace cn::core::state
{

State::State(Manager& _stateManagerRef)
    : m_stateManagerRef{_stateManagerRef}
{
}
    
State::~State() = default;

Return State::update(sf::Time _dt)
{
    m_menuContainer.update(_dt);
    return onUpdate(_dt);
}

void State::draw()
{
    onDraw();
    m_menuContainer.draw(getContext().windowRef);
}

void State::activate()
{
    m_menuContainer.activate();
    onActivate();
    registerEvents(getContext().eventDispatcher, true);
}

void State::deactivate()
{
    registerEvents(getContext().eventDispatcher, false);
    m_menuContainer.deactivate();
    onDeactivate();
}

void State::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    m_menuContainer.registerEvents(_dispatcher, _isBeingRegistered);
    onRegisterEvents(_dispatcher, _isBeingRegistered);
}

void State::push(StateId _stateId)
{
    m_stateManagerRef.pushState(_stateId);
}

void State::pop()
{
    m_stateManagerRef.popState();
}

void State::clear()
{
    m_stateManagerRef.clearStates();
}

core::Context& State::getContext()
{
    return m_stateManagerRef.getContext();
}

core::object::Container& State::getMenuContainer()
{
    return m_menuContainer;
}

} // namespace cn::core::state
