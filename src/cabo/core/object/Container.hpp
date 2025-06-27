#pragma once

#include "core/Context.hpp"
#include "core/object/Object.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <functional>
#include <memory>
#include <vector>

namespace cn::core::object
{

class Container final
{
public:
    using Id = unsigned;
    using SortCallback = std::function<bool(const Object&, const Object&)>;

    Container();
    Container(SortCallback _sortCallback);

    void update(sf::Time _dt);
    void draw(sf::RenderWindow& _windowRef);
    void activate();
    void deactivate();
    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

    void add(std::shared_ptr<Object> _object);

    void processChanges(core::Context& _context);

    bool isSorted() const { return m_isSorted; }

private:
    const bool m_isSorted = false;
    SortCallback m_sortCallback{};
    std::vector<std::shared_ptr<Object>> m_objects;
    std::vector<std::shared_ptr<Object>> m_newObjects;
};

} // namespace cn::core::object
