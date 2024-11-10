#include "menu/Container.hpp"

namespace cn::menu
{

Container::~Container() = default;

void Container::handleEvent(const sf::Event& _event)
{
    for (auto& item : m_items)
        item->handleEvent(_event);
}

void Container::update(sf::Time _dt)
{
    for (auto& item : m_items)
        item->update(_dt);
}

void Container::draw(sf::RenderWindow& _windowRef)
{
    for (auto& item : m_items)
        item->draw(_windowRef);
}

void Container::activate()
{
    for (auto& item : m_items)
        item->activate();
}

void Container::deactivate()
{
    for (auto& item : m_items)
        item->deactivate();
}

void Container::add(std::shared_ptr<Item> _item)
{
    m_items.emplace_back(_item);
}

} // namespace cn::menu
