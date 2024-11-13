#include "state/State.hpp"
#include "state/StateManager.hpp"

namespace cn::state
{

State::State(StateManager& _stateManagerRef)
    : m_stateManagerRef{_stateManagerRef}
{
}
    
State::~State() = default;

State::Return State::handleEvent(const sf::Event& _event)
{
    m_menuContainer.handleEvent(_event);
    return onHandleEvent(_event);
}

State::Return State::update(sf::Time _dt)
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
}

void State::deactivate()
{
    m_menuContainer.deactivate();
    onDeactivate();
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

} // namespace cn::state
