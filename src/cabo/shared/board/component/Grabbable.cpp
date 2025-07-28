#include "shared/board/component/Grabbable.hpp"

namespace cn::shared::board::component
{

Grabbable::Grabbable(Object& _parent)
    : Component(_parent)
{
}

void Grabbable::grab()
{
    m_isGrabbed = true;
}

void Grabbable::release()
{
    m_isGrabbed = false;
}

} // namespace cn::shared::board::component
