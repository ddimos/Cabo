#include "core/object/Container.hpp"

#include <algorithm>

namespace cn::core::object
{

Container::Container()
    : m_isSorted(false)
{
}

Container::Container(SortCallback _sortCallback)
    : m_isSorted(true)
    , m_sortCallback(_sortCallback)
{
}

void Container::update(sf::Time _dt)
{
    if (m_isSorted)
    {
        std::sort(m_objects.begin(), m_objects.end(), 
            [this](const std::shared_ptr<Object>& _left, const std::shared_ptr<Object>& _right){
                return m_sortCallback(*_left, *_right);
            }
        );
    }
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
