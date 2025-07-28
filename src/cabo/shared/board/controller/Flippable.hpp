#pragma once
#include "shared/board/controller/Controller.hpp"
#include "shared/board/component/Flippable.hpp"

namespace cn::shared::board::controller
{

class Flippable : public Controller<component::Flippable>
{
public:
    Flippable() = default;

    component::Flippable* findObjectToFlip(sf::Vector2f _position);
    void flipObject(component::Flippable& _component);
};

} // namespace cn::shared::board::controller
