#include "shared/game/controller/Flippable.hpp"

namespace cn::shared::game::controller
{

component::Flippable* Flippable::findObjectToFlip(sf::Vector2f _position)
{
    return getTopObject(_position);
}

void Flippable::flipObject(component::Flippable& _component)
{
    if (_component.isFaceUp())
        _component.turnDown();
    else
        _component.turnUp();
}

void Flippable::turnUp(component::Flippable& _component)
{
    if (!_component.isFaceUp())
        _component.turnUp();
}

void Flippable::turnDown(component::Flippable& _component)
{
    if (_component.isFaceUp())
        _component.turnDown();
}

} // namespace cn::shared::game::controller
