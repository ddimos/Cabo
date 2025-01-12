#include "core/object/Container.hpp"

namespace cn::core::object
{

void Container::update(sf::Time _dt)
{    
    for (auto& object : m_objects)
        object->update(_dt);
}

void Container::draw(sf::RenderWindow& _windowRef)
{
    for (auto& object : m_objects)
        object->draw(_windowRef);
}

void Container::activate()
{
    for (auto& object : m_objects)
    {
        if (object->isAutoActivated())
            object->activate();
    }
}

void Container::deactivate()
{
    for (auto& object : m_objects)
    {
        if (object->isAutoActivated())
            object->deactivate();
    }
}

void Container::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    for (auto& object : m_objects)
        object->registerEvents(_dispatcher, _isBeingRegistered);
}

void Container::add(std::shared_ptr<Object> _object)
{
    m_newObjects.emplace_back(_object);
}

void Container::processChanges(core::Context& _context)
{
    auto& eventDispatcherRef = _context.get<core::event::Dispatcher>();
    for (auto& object : m_newObjects)
    {
        if (object->isAutoActivated())
        {
            object->activate();
            object->registerEvents(eventDispatcherRef, true);
        }
        m_objects.push_back(object);
    }
    m_newObjects.clear();

    for (auto& object : m_objects)
    {
        if (object->wantsActivated() && !object->isActivated())
        {
            object->activate();
            object->registerEvents(eventDispatcherRef, true);
            object->m_desiredState = Object::DesiredState::None;
        }
        else if (object->wantsDeactivated() && object->isActivated())
        {
            object->registerEvents(eventDispatcherRef, false);
            object->deactivate();
            object->m_desiredState = Object::DesiredState::None;
        }
    }
}

} // namespace cn::core::object
