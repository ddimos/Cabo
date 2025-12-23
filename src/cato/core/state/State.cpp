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
    // TODO don't retrieve the value each frame
    auto& windowRef = getContext().get<sf::RenderWindow>();
    onDraw();
    for (auto& [id, container] : m_containers)
        container.draw(windowRef);
}

void State::activate()
{
    auto& eventDispatcherRef = getContext().get<core::event::Dispatcher>();
    for (auto& [id, container] : m_containers)
        container.activate();
    onActivate();
    registerEvents(eventDispatcherRef, true);
}

void State::deactivate()
{
    auto& eventDispatcherRef = getContext().get<core::event::Dispatcher>();
    registerEvents(eventDispatcherRef, false);
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

void State::createContainer(object::Container::Id _id)
{
    CN_ASSERT(!m_containers.contains(_id));
    m_containers.emplace(_id, object::Container{});
}

void State::createSortedContainer(object::Container::Id _id, object::Container::SortCallback _sortCallback)
{
    CN_ASSERT(!m_containers.contains(_id));
    m_containers.emplace(_id, object::Container{ _sortCallback });
}

object::Container& State::getContainer(object::Container::Id _id)
{
    CN_ASSERT(m_containers.contains(_id));
    return m_containers.at(_id);
}

} // namespace cn::core::state
