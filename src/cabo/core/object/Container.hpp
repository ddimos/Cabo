#pragma once

#include "core/Types.hpp"
#include "core/object/Object.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>
#include <vector>

namespace cn::core::object
{

using ContainerId = unsigned;

class Container final
{
public:
    enum class Type
    {
        Game,
        Menu
    };

    void update(sf::Time _dt);
    void draw(sf::RenderWindow& _windowRef);
    void activate();
    void deactivate();
    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

    void add(std::shared_ptr<Object> _object);

    void processChanges(core::Context& _context);

private:
    std::vector<std::shared_ptr<Object>> m_objects;
    std::vector<std::shared_ptr<Object>> m_newObjects;
};

} // namespace cn::core::object
