#pragma once

#include "core/object/Object.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>
#include <vector>

namespace cn::core::object
{

class Container final
{
public:
    void handleEvent(const sf::Event& _event);
    void update(sf::Time _dt);
    void draw(sf::RenderWindow& _windowRef);
    void activate();
    void deactivate();

    void add(std::shared_ptr<Object> _object);

private:
    std::vector<std::shared_ptr<Object>> m_objects;
};

} // namespace cn::core::object
