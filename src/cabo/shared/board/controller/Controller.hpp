#pragma once

#include "shared/board/Object.hpp"
#include "shared/board/component/Component.hpp"

#include <SFML/System/Vector2.hpp>

#include <type_traits>
#include <vector>

namespace cn::shared::board::controller
{

template <typename TComponent>
class Controller
{
    static_assert(std::is_base_of<component::Component, TComponent>::value);

public:
    Controller() = default;

    void add(TComponent& _component);

protected:
    TComponent* getTopObject(sf::Vector2f _position);

    std::vector<TComponent*> m_components;
};

template <typename TComponent>
void Controller<TComponent>::add(TComponent& _component)
{
    m_components.push_back(&_component);
}

template <typename TComponent>
TComponent* Controller<TComponent>::getTopObject(sf::Vector2f _position)
{
    TComponent* topComponent = nullptr;
    for (auto& component : m_components)
    {
        if (!component->getParent().contains(_position))
            continue;

        if (!topComponent)
        {
            topComponent = component;
            continue;
        }

        if (topComponent->getParent().getZIndex() < component->getParent().getZIndex())
            topComponent = component;
    }
    return topComponent;
}

} // namespace cn::shared::game::controller
