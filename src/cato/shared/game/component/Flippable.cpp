#include "shared/game/component/Flippable.hpp"

namespace cn::shared::game::component
{

Flippable::Flippable(object::Object& _parent)
    : Component(_parent)
{
}

void Flippable::turnUp()
{
    m_isFaceUp = true;
}

void Flippable::turnDown()
{
    m_isFaceUp = false;
}

} // namespace cn::shared::game::component
