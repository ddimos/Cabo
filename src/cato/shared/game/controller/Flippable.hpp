#pragma once
#include "shared/game/controller/Controller.hpp"
#include "shared/game/component/Flippable.hpp"

namespace cn::shared::game::controller
{

class Flippable : public Controller<component::Flippable>
{
public:
    Flippable() = default;

    component::Flippable* findObjectToFlip(sf::Vector2f _position);
    void flipObject(component::Flippable& _component);
    void turnUp(component::Flippable& _component);
    void turnDown(component::Flippable& _component);
};

} // namespace cn::shared::game::controller
