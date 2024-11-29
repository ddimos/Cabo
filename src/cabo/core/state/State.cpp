#include "core/state/State.hpp"
#include "core/state/Manager.hpp"
#include "core/Assert.hpp"

namespace cn::core::state
{

State::State(Manager& _stateManagerRef)
    : m_stateManagerRef{_stateManagerRef}
{
}
    
State::~State() = default;

Return State::update(sf::Time _dt)
{
    for (auto& [id, container] : m_containers)
        container.processChanges(getContext());

    for (auto& [id, container] : m_containers)
        container.update(_dt);

    return onUpdate(_dt);
}

void State::draw()
{
    onDraw();
    for (auto& [id, container] : m_containers)
        container.draw(getContext().windowRef);
}

void State::activate()
{
    for (auto& [id, container] : m_containers)
        container.activate();
    onActivate();
    registerEvents(getContext().eventDispatcher, true);
}

void State::deactivate()
{
    registerEvents(getContext().eventDispatcher, false);
    for (auto& [id, container] : m_containers)
        container.deactivate();
    onDeactivate();
}

void State::registerEvents(event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    for (auto& [id, container] : m_containers)
        container.registerEvents(_dispatcher, _isBeingRegistered);
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

Context& State::getContext()
{
    return m_stateManagerRef.getContext();
}

void State::createContainer(object::Container::Type _type)
{
    CN_ASSERT(!m_containers.contains(_type));
    m_containers.emplace(_type, object::Container{});
}

object::Container& State::getContainer(object::Container::Type _type)
{
    CN_ASSERT(m_containers.contains(_type));
    return m_containers.at(_type);
}

} // namespace cn::core::state
