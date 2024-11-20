#include "core/object/Container.hpp"

namespace cn::core::object
{

void Container::handleEvent(const sf::Event& _event)
{
    for (auto& object : m_objects)
        object->handleEvent(_event);
}

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

void Container::add(std::shared_ptr<Object> _object)
{
    m_objects.emplace_back(_object);
}

} // namespace cn::core::object
