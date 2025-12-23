#include "shared/game/component/Attachable.hpp"

namespace cn::shared::game::component
{

Attachable::Attachable(object::Object& _parent)
    : Component(_parent)
{
}

int Attachable::getRadius() const
{
    auto size = getParent().getSize();
    return std::max(size.x, size.y);
}

void Attachable::attach()
{
    // TODO
}

void Attachable::detach()
{
}

} // namespace cn::shared::game::component
