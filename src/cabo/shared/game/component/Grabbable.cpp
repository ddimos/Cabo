#include "shared/game/component/Grabbable.hpp"

namespace cn::shared::game::component
{

Grabbable::Grabbable(object::Object& _parent)
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

} // namespace cn::shared::game::component
