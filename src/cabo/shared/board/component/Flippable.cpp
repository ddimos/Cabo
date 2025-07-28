#include "shared/board/component/Flippable.hpp"

namespace cn::shared::board::component
{

Flippable::Flippable(Object& _parent)
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

} // namespace cn::shared::board::component
