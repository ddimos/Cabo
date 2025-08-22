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

} // namespace cn::shared::game::controller
