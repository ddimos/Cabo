#include "shared/board/component/Attachable.hpp"

namespace cn::shared::board::component
{

Attachable::Attachable(Object& _parent)
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

} // namespace cn::shared::board::component
