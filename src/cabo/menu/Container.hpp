#pragma once

#include "menu/Item.hpp"

#include <memory>
#include <vector>

namespace sf
{
    class Event;
    class RenderWindow;
}

namespace cn::menu
{
    
class Container final
{
public:
    virtual ~Container();

public:
    void handleEvent(const sf::Event& _event);
    void update(sf::Time _dt);
    void draw(sf::RenderWindow& _windowRef);
    void activate();
    void deactivate();

    void add(std::shared_ptr<Item> _item);

private:
    std::vector<std::shared_ptr<Item>> m_items;
};

} // namespace cn::menu
